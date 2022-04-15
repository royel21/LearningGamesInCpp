#include "ScriptSystem.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Time/Timer.h>
#include <Input/Input.h>

#include <Graphics/Camera2D.h>
#include <Graphics/GLheaders.h>

#include <Assets/Assets.h>

#include <Math/Vectors.h>

namespace Plutus
{
    int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description)
    {
        /* std::cout << "An exception occurred in a function, here's what it says ";
         if (maybe_exception)
         {
             std::cout << "(straight from the exception): ";
             const std::exception& ex = *maybe_exception;
             std::cout << ex.what() << std::endl;
         }
         else
         {
             std::cout << "(from the description parameter): ";
             std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
             std::cout << std::endl;
         }*/
        return sol::stack::push(L, description);
    }

    ScriptSystem::ScriptSystem(Camera2D* camera) {
        mGlobalLua.open_libraries(
            sol::lib::base,
            sol::lib::math
        );

        mGlobalLua.set_exception_handler(&my_exception_handler);

        mGlobalLua.set("input", Input::get());
        mGlobalLua.set("getMillis", &Timer::millis);
        mGlobalLua.set("assetManager", AssetManager::get());

        /*****************************Register EntityManager**********************************************/
        auto scene_table = mGlobalLua.new_usertype<Scene>("Scene");
        scene_table["getEntity"] = &Scene::getEntityByName;

        /*****************************Register Input manager**********************************************/
        auto input = mGlobalLua.new_usertype<Input>("Input");
        input["onKeyDown"] = &Input::onKeyDown;
        input["onKeyPressed"] = &Input::onKeyPressed;

        auto lua_vec2 = mGlobalLua.new_usertype<vec2f>("vec2f", sol::constructors<vec2f(), vec2f(float, float), vec2f(int, int)>());
        lua_vec2["x"] = &vec2f::x;
        lua_vec2["y"] = &vec2f::y;

        registerAssets();
        registerEntity();
        registerComponents();
    }

    void ScriptSystem::init(Scene* scene)
    {
        mScene = scene;
        //Scene References
        mGlobalLua.set("scene", scene);

        auto view = scene->getRegistry()->view<ScriptComponent>();

        for (auto [ent, script] : view.each()) {
            script.init(mGlobalLua, { ent, scene });
        }
    }

    void ScriptSystem::update(float dt)
    {
        auto view = mScene->getRegistry()->view<ScriptComponent>();

        for (auto [ent, script] : view.each()) {
            script.update(dt);
        }
    }

    void ScriptSystem::registerAssets()
    {
        /*****************************Register AssetManager**********************************************/
        auto assetManager_table = mGlobalLua.new_usertype<AssetManager>("AssetManager",
            "addSound", sol::overload(
                &AssetManager::addAsset<Sound, std::string>,
                &AssetManager::addAsset<Sound, std::string, int>
            ));
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
            Texture(const std::string&, int, int, int),
            Texture(const std::string&, int, int, int, GLint, GLint)>()
            );
        texture_table["getUV"] = sol::overload(&Texture::getUV<int>, &Texture::getUV<float, float, float, float>);

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