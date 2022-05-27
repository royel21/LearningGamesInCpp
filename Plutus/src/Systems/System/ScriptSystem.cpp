#include "ScriptSystem.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Time/Timer.h>
#include <Input/Input.h>

#include <Graphics/Camera2D.h>
#include <Graphics/GLheaders.h>

#include <Assets/Assets.h>

#include <Math/Vectors.h>

#include <Core/Project.h>

namespace Plutus
{

    void ScriptSystem::init(Project* project)
    {
        mProject = project;
        //Scene References
        mGlobalLua.set("scene", project->scene.get());

        auto view = project->scene->getRegistry()->view<ScriptComponent>();

        for (auto [ent, script] : view.each()) {
            script.init(mGlobalLua, { ent, project->scene.get() });
        }
    }

    ScriptSystem::ScriptSystem(Camera2D* camera) : ISystem(camera) {
        mGlobalLua.open_libraries(
            sol::lib::base,
            sol::lib::math
        );

        mGlobalLua.set("input", Input::get());
        mGlobalLua.set("getMillis", &Time::millis);
        mGlobalLua.set("assetManager", AssetManager::get());

        /*****************************Register EntityManager**********************************************/
        auto scene_table = mGlobalLua.new_usertype<Scene>("Scene");
        scene_table["getEntity"] = sol::overload(&Scene::getEntityByName, &Scene::getEntity);

        /*****************************Register Input manager**********************************************/
        auto input = mGlobalLua.new_usertype<Input>("Input");
        input["onKeyDown"] = &Input::onKeyDown;
        input["onKeyPressed"] = &Input::onKeyPressed;

        auto lua_vec2 = mGlobalLua.new_usertype<Vec2f>("Vec2f", sol::constructors<Vec2f(), Vec2f(float, float), Vec2f(int, int)>());
        lua_vec2["x"] = &Vec2f::x;
        lua_vec2["y"] = &Vec2f::y;
        lua_vec2["unit"] = &Vec2f::unit;
        lua_vec2["getDirection"] = &Vec2f::getDirection;

        auto lua_vec4 = mGlobalLua.new_usertype<Vec4f>("Vec4f", sol::constructors<Vec4f(), Vec4f(float, float, float, float)>());

        lua_vec4["x"] = &Vec4f::x;
        lua_vec4["y"] = &Vec4f::y;
        lua_vec4["z"] = &Vec4f::z;
        lua_vec4["w"] = &Vec4f::w;

        registerAssets();
        registerCamera();
        registerEntity();
        registerComponents();
    }

    void ScriptSystem::update(float dt)
    {
        auto view = mProject->scene->getRegistry()->view<ScriptComponent>();

        for (auto [ent, script] : view.each()) {
            script.update(dt);
        }
    }

    void ScriptSystem::registerAssets()
    {
        /*****************************Register AssetManager**********************************************/
        auto assetManager_table = mGlobalLua.new_usertype<AssetManager>("AssetManager");
        assetManager_table["addSound"] = sol::overload(&AssetManager::addAsset<Sound, std::string>, &AssetManager::addAsset<Sound, std::string, int>);
        assetManager_table["removeSound"] = &AssetManager::removeAsset<Sound>;

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
        texture_table["getUV"] = sol::overload(&Texture::getUV<int>, &Texture::getUV<float, float, float, float>);
    }

    void ScriptSystem::registerCamera() {
        auto lua_camera = mGlobalLua.new_usertype<Camera2D>("Camera2D");
        lua_camera["getPosition"] = &Camera2D::getPosition;
        lua_camera["setBounds"] = &Camera2D::setBounds;

        lua_camera["setPosition"] = sol::overload(
            [&](float x, float y) {mCamera->setPosition(x, y); },
            [&](const Vec2f& pos) {mCamera->setPosition(pos); }
        );

        lua_camera["setTarget"] = &Camera2D::setTarget;
        lua_camera["getVPSize"] = &Camera2D::getScaleScreen;

        mGlobalLua["camera"] = mCamera;
    }

    void ScriptSystem::registerEntity()
    {
        /*****************************Register Entity**********************************************/
        auto entity = mGlobalLua.new_usertype<Entity>("Entity");
        entity["getTransform"] = &Entity::getComponent<TransformComponent>;
        entity["getTileMap"] = &Entity::getComponent<TileMapComponent>;
        entity["getAnimate"] = &Entity::getComponent<AnimationComponent>;
        entity["getSprite"] = &Entity::getComponent<SpriteComponent>;
        entity["getRigidBody"] = &Entity::getComponent<RigidBodyComponent>;
        entity["getVelocity"] = &Entity::getComponent<VelocityComponent>;

        entity["getName"] = &Entity::getName;
        entity["getPosition"] = &Entity::getPosition;
        entity["getCenter"] = &Entity::getCenter;
        entity["getDirection"] = &Entity::getDirection;
    }

    void ScriptSystem::registerComponents()
    {
        /*****************************Register Animation**********************************************/
        auto animate = mGlobalLua.new_usertype<AnimationComponent>("Animation");
        animate["play"] = &AnimationComponent::play;
        animate["setLoop"] = &AnimationComponent::setLoop;
        animate["loop"] = &AnimationComponent::loop;
        animate["addSeq"] = &AnimationComponent::addSeq;
        animate["addSeq2"] = &AnimationComponent::addSequence;
        animate["setTexture"] = &AnimationComponent::setTexture;


        /*******************************************Register Animation Sequence**********************************************/
        auto sequence = mGlobalLua.new_usertype<Sequence>("Sequence",
            sol::constructors<Sequence(),
            Sequence(const std::string&, std::vector<uint32_t>, int)>());

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
    }
}