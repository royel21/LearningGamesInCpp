#include "ScriptSystem.h"
#include "PhysicSystem.h"
#include "../SystemManager.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Time/Timer.h>
#include <Input/Input.h>
#include <Core/Project.h>
#include <Utils/FileIO.h>
#include <Assets/Assets.h>

#include <Graphics/Camera2D.h>
#include <Graphics/GLheaders.h>

#include <Events/CollisionEvent.h>
#include <Serialize/SceneLoader.h>


namespace Plutus
{
    using EntList = std::vector<Entity>;

    bool containsEnt(EntList ents, uint32_t e) {
        auto found = std::find_if(ents.begin(), ents.end(),
            [e](const Entity& ent) { return ent == e; });
        return found != ents.end();
    }

    EntList getEntList(Project* proj, const std::vector<uint32_t> list) {
        std::vector<Entity> ents;
        ents.reserve(list.size());
        for (auto e : list) {
            if (!containsEnt(ents, e)) {
                ents.push_back(proj->scene->getEntity(e));
            }
        }
        return ents;
    }

    Vec2f getDirection(float x, float y, const std::string& dir, float offset) {

        if (dir == "right") {
            x += offset;
        }
        else if (dir == "left") {
            x -= offset;
        }
        else if (dir == "up") {
            y += offset;
        }
        else if (dir == "down") {
            y -= offset;
        }
        return { x, y };
    }

    ScriptSystem::ScriptSystem(Camera2D* camera) : ISystem(camera) {

        mGlobalLua.open_libraries(
            sol::lib::base,
            sol::lib::math,
            sol::lib::package
        );
        registerEntity();
        registerAssets();
        registerCamera();
        registerComponents();
        registerGlobals();
    }

    void ScriptSystem::registerGlobals() {
        mGlobalLua.set("input", Input::get());
        mGlobalLua.set("getMillis", &Time::millis);
        mGlobalLua.set("assetManager", AssetManager::get());

        /*****************************Register EntityManager**********************************************/
        auto scene_table = mGlobalLua.new_usertype<Scene>("Scene");
        scene_table["getEntity"] = sol::overload(&Scene::getEntityByName, &Scene::getEntity);
        scene_table["removeEntity"] = sol::overload(
            sol::resolve<void(Entity)>(&Scene::removeEntity),
            sol::resolve<void(uint32_t)>(&Scene::removeEntity)
        );

        scene_table["createEntity"] = &Scene::createEntity;

        mGlobalLua["SceneLoader"] = &SceneLoader::loadFromPath;

        /*****************************Register Input manager**********************************************/
        auto input = mGlobalLua.new_usertype<Input>("Input");
        input["onKeyDown"] = &Input::onKeyDown;
        input["onKeyPressed"] = &Input::onKeyPressed;

        auto lua_vec2 = mGlobalLua.new_usertype<Vec2f>(
            "Vec2f", sol::constructors<Vec2f(),
            Vec2f(float, float), Vec2f(int, int)>(),
            sol::meta_function::multiplication, sol::resolve<Vec2f(float) const>(&Vec2f::operator*),
            sol::meta_function::addition,
            sol::overload(
                sol::resolve<Vec2f(const Vec2f&) const>(&Vec2f::operator+),
                sol::resolve<Vec2f(const float) const>(&Vec2f::operator+)
            ),
            sol::meta_function::subtraction,
            sol::overload(sol::resolve<Vec2f(const Vec2f&) const>(&Vec2f::operator-)),
            sol::meta_function::unary_minus,
            sol::overload(sol::resolve<Vec2f(const float) const>(&Vec2f::operator-)),
            sol::meta_function::equal_to, &Vec2f::operator==,
            sol::meta_function::less_than, &Vec2f::operator<,
            sol::meta_function::less_than_or_equal_to, &Vec2f::operator<=
            );

        lua_vec2["x"] = &Vec2f::x;
        lua_vec2["y"] = &Vec2f::y;
        lua_vec2["dot"] = &Vec2f::dot;
        lua_vec2["unit"] = &Vec2f::unit;
        lua_vec2["cross"] = &Vec2f::cross;
        lua_vec2["normal"] = &Vec2f::normal;
        lua_vec2["length"] = &Vec2f::length;
        lua_vec2["invLength"] = &Vec2f::invLength;
        lua_vec2["getDirection"] = &Vec2f::getDirection;

        auto lua_vec4 = mGlobalLua.new_usertype<Vec4f>("Vec4f", sol::constructors<Vec4f(), Vec4f(float, float, float, float)>());

        lua_vec4["x"] = &Vec4f::x;
        lua_vec4["y"] = &Vec4f::y;
        lua_vec4["z"] = &Vec4f::z;
        lua_vec4["w"] = &Vec4f::w;


        mGlobalLua["initScript"] = [&](uint32_t entId) {
            auto script = mProject->scene->getComponent<ScriptComponent>(entId);
            if (script) script->init(mGlobalLua, { entId, mProject->scene.get() });
        };

        mGlobalLua["getDirection"] = sol::overload(
            [&](const Vec2f& pos, const std::string& dir, float offset) { return getDirection(pos.x, pos.y, dir, offset); },
            &getDirection
        );
    }

    void ScriptSystem::init()
    {
        //Scene References
        mGlobalLua.set("scene", mProject->scene.get());

        mPhysicSys = mSysManager->getSystem<PhysicSystem>();
        if (mPhysicSys) {
            mPhysicSys->AddListener(this);
            registerPhysics();
        }

        auto initScript = AssetManager::get()->getBaseDir() + "/assets/scripts/init.lua";

        if (FileIO::exists(initScript)) {
            mGlobalLua.script_file(initScript);
        }

        auto view = mProject->scene->getRegistry()->view<ScriptComponent>();

        for (auto [ent, script] : view.each()) {
            script.init(mGlobalLua, { ent, mProject->scene.get() });
        }
    }

    void ScriptSystem::update(float dt)
    {
        auto view = mProject->scene->getRegistry()->view<ScriptComponent>();

        for (auto [ent, script] : view.each()) {
            script.update(dt);
        }
    }


    uint32_t ScriptSystem::castRay(const Vec2f& start, const Vec2f& end, uint32_t mask) {
        uint32_t entId = 0;
        mPhysicSys->CastRay(start, end,
            [&](b2Fixture* fixture, Vec2f point, Vec2f normal, float fraction) -> float
            {
                if (fixture->GetFilterData().categoryBits & mask) {
                    entId = fixture->GetBody()->GetUserData().pointer;
                    return 0;
                }
                return 1;
            }
        );
        return entId;
    }

    void ScriptSystem::registerPhysics() {

        mGlobalLua["castRay"] = sol::overload(
            [&](Vec2f start, Vec2f end) { return Entity{ castRay(start, end), mProject->scene.get() }; },
            [&](Vec2f start, Vec2f end, uint32_t mask) { return Entity{ castRay(start, end, mask), mProject->scene.get() }; }
        );

        mGlobalLua["queryWorld"] = sol::overload(
            [&](float x, float y, float size) {
                float half = size * 0.5f;
                return getEntList(mProject, mPhysicSys->queryWorld({ x - half, y - half, size, size }, 0xffff));
            },
            [&](float x, float y, float size, uint32_t mask)
            {
                float half = size * 0.5f;
                return getEntList(mProject, mPhysicSys->queryWorld({ x - half, y - half, size, size }, mask));
            },
                [&](float x, float y, float size, uint32_t mask, float offset, const std::string& dir)
            {
                auto pos = getDirection(x, y, dir, offset);
                float half = size * 0.5f;
                return getEntList(mProject, mPhysicSys->queryWorld({ pos.x - half, pos.y - half, size, size }, mask));
            }
            );
    }

    void ScriptSystem::registerAssets()
    {
        /*****************************Register AssetManager**********************************************/
        auto assetManager_table = mGlobalLua.new_usertype<AssetManager>("AssetManager");
        assetManager_table["addSound"] = sol::overload(
            &AssetManager::addAsset<Sound, std::string>,
            &AssetManager::addAsset<Sound, std::string, int>
        );
        assetManager_table["removeSound"] = &AssetManager::removeAsset<Sound>;

        /**************************Register Font Asset*************************************************/
        mGlobalLua.new_usertype<Font>("Font", sol::constructors<Font(const std::string&, int)>());

        /**************************Register Font Asset*************************************************/
        mGlobalLua.new_usertype<Script>("Script", sol::constructors<Script(const std::string&)>());

        /**************************Register Sound Asset*************************************************/
        auto sound_table = mGlobalLua.new_usertype<Sound>("Sound",
            sol::constructors<Sound(),
            Sound(const std::string&),
            Sound(const std::string&, int)>()
            );

        sound_table["play"] = &Sound::play;
        sound_table["pause"] = &Sound::pause;
        sound_table["stop"] = &Sound::stop;
        sound_table["path"] = &Sound::mPath;
        sound_table["type"] = &Sound::mType;

        /**************************Register Texture Asset*************************************************/
        auto texture_table = mGlobalLua.new_usertype<Texture>("Texture",
            sol::constructors<Texture(const std::string&),
            Texture(const std::string&, int, int),
            Texture(const std::string&, int, int, GLint)>()
            );

        texture_table["getUV"] = sol::overload(
            sol::resolve<Vec4f(int)>(&Texture::getUV),
            sol::resolve<Vec4f(float, float, float, float)>(&Texture::getUV)
        );
    }

    void ScriptSystem::registerCamera() {
        auto lua_camera = mGlobalLua.new_usertype<Camera2D>("Camera2D");
        lua_camera["getPosition"] = &Camera2D::getPosition;
        lua_camera["setBounds"] = &Camera2D::setBounds;

        lua_camera["setPosition"] = sol::overload(
            sol::resolve<void(float, float)>(&Camera2D::setPosition),
            sol::resolve<void(const Vec2f&)>(&Camera2D::setPosition)
        );

        lua_camera["setTarget"] = &Camera2D::setTarget;
        lua_camera["getVPSize"] = &Camera2D::getScaleScreen;

        mGlobalLua["camera"] = mCamera;
    }

    void ScriptSystem::registerEntity()
    {
        /*****************************Register Entity**********************************************/
        auto entity = mGlobalLua.new_usertype<Entity>("Entity",
            sol::meta_function::equal_to,
            sol::overload(sol::resolve<bool(const Entity&) const>(&Entity::operator==))
            );

        entity["getId"] = &Entity::getId;
        entity["isValid"] = &Entity::isValid;

        entity["getTransform"] = &Entity::getComponent<TransformComponent>;
        entity["getTileMap"] = &Entity::getComponent<TileMapComponent>;
        entity["getAnimate"] = &Entity::getComponent<AnimationComponent>;
        entity["getSprite"] = &Entity::getComponent<SpriteComponent>;
        entity["getRigidBody"] = &Entity::getComponent<RigidBodyComponent>;
        entity["getVelocity"] = &Entity::getComponent<VelocityComponent>;


        entity["addTransform"] = sol::overload(
            &Entity::addComponent<TransformComponent, float, float, int, int>,
            &Entity::addComponent<TransformComponent, float, float, int, int, float>,
            &Entity::addComponent<TransformComponent, float, float, int, int, float, int>,
            &Entity::addComponent<TransformComponent, float, float, int, int, float, int, bool>
        );

        entity["addTileMap"] = &Entity::addComponent<TileMapComponent>;
        entity["addAnimate"] = &Entity::addComponent<AnimationComponent>;
        entity["addSprite"] = &Entity::addComponent<SpriteComponent>;
        entity["addRigidBody"] = &Entity::addComponent<RigidBodyComponent>;
        entity["addVelocity"] = &Entity::addComponent<VelocityComponent>;
        entity["addScript"] = &Entity::addComponent<ScriptComponent>;

        entity["getName"] = &Entity::getName;
        entity["getPosition"] = &Entity::getPosition;
        entity["getCenter"] = &Entity::getCenter;
        entity["getDirection"] = &Entity::getDirection;
        entity["getDistance"] = &Entity::getDistance;
    }

    void ScriptSystem::registerComponents()
    {
        /*****************************Register Animation**********************************************/
        auto animate = mGlobalLua.new_usertype<AnimationComponent>("Animation");
        animate["play"] = &AnimationComponent::play;
        animate["setLoop"] = &AnimationComponent::setLoop;
        animate["loop"] = &AnimationComponent::loop;

        animate["addSeq"] = sol::overload(
            sol::resolve<void(const std::string&, Frames)>(&AnimationComponent::addSeq),
            sol::resolve<void(const std::string&, Frames, int)>(&AnimationComponent::addSeq),
            sol::resolve<void(const std::string&, Frames, int, bool)>(&AnimationComponent::addSeq)
        );

        animate["addSeq2"] = &AnimationComponent::addSequence;
        animate["setTexture"] = &AnimationComponent::setTexture;


        /*******************************************Register Animation Sequence**********************************************/
        auto sequence = mGlobalLua.new_usertype<Sequence>("Sequence",
            sol::constructors<Sequence(),
            Sequence(const std::string&, std::vector<uint32_t>, int)>()
            );

        sequence["frames"] = &Sequence::mFrames;
        sequence["texId"] = &Sequence::mTexId;
        sequence["time"] = &Sequence::mSeqTime;

        /*****************************Register Transform**********************************************/
        auto transform = mGlobalLua.new_usertype<TransformComponent>("Transform");
        transform["x"] = &TransformComponent::x;
        transform["y"] = &TransformComponent::y;
        transform["w"] = &TransformComponent::w;
        transform["h"] = &TransformComponent::h;
        transform["rotation"] = &TransformComponent::r;
        transform["layer"] = &TransformComponent::layer;
        transform["sortY"] = &TransformComponent::sortY;
        transform["getRect"] = &TransformComponent::getRect;

        /*****************************Register Tilemap and Tile**********************************************/
        auto tileMap = mGlobalLua.new_usertype<TileMapComponent>("TileMap");
        tileMap["tiles"] = &TileMapComponent::mTiles;

        auto tile = mGlobalLua.new_usertype<Tile>("Tile");
        tile["x"] = &Tile::x;
        tile["y"] = &Tile::y;

        /*****************************Register Rigid Body**********************************************/
        auto rigidBody = mGlobalLua.new_usertype<RigidBodyComponent>("RigidBody");
        rigidBody["applyImpulse"] = &RigidBodyComponent::ApplyImpulse;
        rigidBody["applyForce"] = &RigidBodyComponent::ApplyForce;
        rigidBody["setVelocity"] = &RigidBodyComponent::setVelocity;
        rigidBody["getVelocity"] = &RigidBodyComponent::getVelocity;
        rigidBody["setMaxVelocity"] = &RigidBodyComponent::setMaxVel;

        /*****************************Register Velocity**********************************************/
        auto velocity = mGlobalLua.new_usertype<VelocityComponent>("Velocity");

        velocity["velocity"] = &VelocityComponent::mVelocity;
        velocity["setVel"] = &VelocityComponent::setVel;

        auto luascript = mGlobalLua.new_usertype<ScriptComponent>("Script",
            sol::constructors<ScriptComponent(), ScriptComponent(const std::string&)>()
            );
        luascript["setScript"] = &ScriptComponent::setScript;
    }

    void ScriptSystem::CollisionEvent(uint32_t ent1, bool isSensorA, uint32_t ent2, bool isSensorB, bool collisionStart) {
        auto script1 = mProject->scene->getComponent<ScriptComponent>(ent1);
        auto script2 = mProject->scene->getComponent<ScriptComponent>(ent2);

        if (collisionStart) {
            if (script1) script1->CollisionStart(ent2, isSensorA);
            if (script2) script2->CollisionStart(ent1, isSensorB);
        }
        else {
            if (script1) script1->CollisionEnd(ent2, isSensorA);
            if (script2) script2->CollisionEnd(ent1, isSensorB);
        }
    }
}