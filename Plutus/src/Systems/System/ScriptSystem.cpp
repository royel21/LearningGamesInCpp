#include "ScriptSystem.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Time/Timer.h>
#include <Input/Input.h>

#include <Graphics/Camera2D.h>
#include <Graphics/GLheaders.h>

#include <Assets/SoundEngine.h>

namespace Plutus
{
    int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description)
    {
        // std::cout << "An exception occurred in a function, here's what it says ";
        if (maybe_exception)
        {
            // std::cout << "(straight from the exception): ";
            // const std::exception& ex = *maybe_exception;
            // std::cout << ex.what() << std::endl;
        }
        else
        {
            // std::cout << "(from the description parameter): ";
            // std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
            // std::cout << std::endl;
        }
        return sol::stack::push(L, description);
    }

    ScriptSystem::ScriptSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera) {
        mGlobalLua.open_libraries(sol::lib::base, sol::lib::math);
        mGlobalLua.set_exception_handler(&my_exception_handler);


        mGlobalLua.set("input", Input::get());
        mGlobalLua.set("scene", scene);
        mGlobalLua.set("SoundEngine", &SoundEngine);
        mGlobalLua.set("getMillis", &Timer::millis);
        /*****************************Register EntityManager**********************************************/
        auto scene_table = mGlobalLua.new_usertype<Scene>("Scene");
        scene_table["getEntity"] = &Scene::getEntityByName;

        /**************************Register Audio Engine*************************************************/
        auto audioEngine = mGlobalLua.new_usertype<AudioEngine>("AudioEngine");
        audioEngine["play"] = &AudioEngine::play;
        audioEngine["pause"] = &AudioEngine::pause;
        audioEngine["stop"] = &AudioEngine::stop;

        /*****************************Register Input manager**********************************************/
        auto input = mGlobalLua.new_usertype<Input>("Input");
        input["onKeyDown"] = &Input::onKeyDown;
        input["onKeyPressed"] = &Input::onKeyPressed;

        /*****************************Register Transform**********************************************/
        auto transform = mGlobalLua.new_usertype<Transform>("Transform");
        transform["x"] = &Transform::x;
        transform["y"] = &Transform::y;
        transform["w"] = &Transform::w;
        transform["h"] = &Transform::h;
        transform["rotation"] = &Transform::r;

        /*****************************Register Tilemap and Tile**********************************************/
        auto tileMap = mGlobalLua.new_usertype<TileMap>("TileMap");
        tileMap["tiles"] = &TileMap::mTiles;

        auto tile = mGlobalLua.new_usertype<Tile>("Tile");
        tile["x"] = &Tile::x;
        tile["y"] = &Tile::y;

        /*****************************Register Animation**********************************************/
        auto animate = mGlobalLua.new_usertype<Animation>("Animation");
        animate["play"] = &Animation::play;
        animate["setLoop"] = &Animation::setLoop;
        animate["loop"] = &Animation::loop;
        animate["addSeq"] = &Animation::addSeq;
        animate["addSeq2"] = &Animation::addSequence;
        animate["setTexture"] = &Animation::setTexture;

        /*******************************************Register Animation Sequence**********************************************/
        auto sequence = mGlobalLua.new_usertype<Sequence>("Sequence",
            sol::constructors<Sequence(),
            Sequence(const std::string&, std::vector<uint32_t>, int)>());

        sequence["frames"] = &Sequence::mFrames;
        sequence["texId"] = &Sequence::mTexId;
        sequence["time"] = &Sequence::mSeqTime;

        /*****************************Register Entity**********************************************/
        auto entity = mGlobalLua.new_usertype<Entity>("Entity");
        entity["getTransform"] = &Entity::getComponent<Transform>;
        entity["getTileMap"] = &Entity::getComponent<TileMap>;
        entity["getAnimate"] = &Entity::getComponent<Animation>;
        entity["getSprite"] = &Entity::getComponent<Sprite>;
        entity["getRigidBody"] = &Entity::getComponent<RigidBody>;
        entity["getVelocity"] = &Entity::getComponent<Velocity>;

        /*****************************Register Velocity**********************************************/
        auto rigidBody = mGlobalLua.new_usertype<RigidBody>("RigidBody");
        rigidBody["applyImpulse"] = &RigidBody::ApplyImpulse;
        rigidBody["applyForce"] = &RigidBody::ApplyForce;
        rigidBody["setVelocity"] = &RigidBody::setVelocity;
        /*****************************Register Velocity**********************************************/
        auto velocity = mGlobalLua.new_usertype<Velocity>("Velocity");

        velocity["velocity"] = &Velocity::mVelocity;
        velocity["setVel"] = &Velocity::setVel;
    }

    void ScriptSystem::init()
    {
        auto view = mScene->getRegistry()->view<Script>();

        for (auto [ent, script] : view.each()) {
            script.init(mGlobalLua, { ent, mScene });
        }
    }

    void ScriptSystem::update(float dt)
    {
        auto view = mScene->getRegistry()->view<Script>();

        for (auto [ent, script] : view.each()) {
            script.update(dt);
        }
    }

    void ScriptSystem::destroy() {
        auto view = mScene->getRegistry()->view<Script>();

        for (auto [ent, script] : view.each()) {
            script.destroy();
        }
    }
}