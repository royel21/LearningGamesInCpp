#include "ScriptServer.h"
#include "Components.h"
#include <Time/Timer.h>
#include <Input/Input.h>

namespace Plutus
{
    void my_panic(sol::optional<std::string> maybe_msg)
    {
        std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
        if (maybe_msg)
        {
            const std::string& msg = maybe_msg.value();
            std::cerr << "\terror message: " << msg << std::endl;
        }
        // When this function exits, Lua will exhibit default behavior and abort()
    }

    int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description)
    {
        // L is the lua state, which you can wrap in a state_view if necessary
        // maybe_exception will contain exception, if it exists
        // description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
        std::cout << "An exception occurred in a function, here's what it says ";
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
        }

        // you must push 1 element onto the stack to be
        // transported through as the error object in Lua
        // note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
        // so we push a single string (in our case, the description of the error)
        return sol::stack::push(L, description);
    }

    ScriptServer* ScriptServer::get()
    {
        static ScriptServer server;
        return &server;
    }

    ScriptServer::ScriptServer()
    {

        lua.open_libraries(sol::lib::base, sol::lib::math);
        lua.set_exception_handler(&my_exception_handler);

        lua.set("getMillis", &Timer::millis);
        auto input = lua.new_usertype<Input>("Input");
        input["onKeyDown"] = &Input::onKeyDown;
        input["onKeyPressed"] = &Input::onKeyPressed;

        auto transform = lua.new_usertype<Transform>("Transform");
        transform["x"] = &Transform::x;
        transform["y"] = &Transform::y;
        transform["w"] = &Transform::w;
        transform["h"] = &Transform::h;
        transform["r"] = &Transform::r;

        auto tile = lua.new_usertype<Tile>("Tile");
        tile["x"] = &Tile::x;
        tile["y"] = &Tile::y;

        auto animate = lua.new_usertype<Animation>("Animation");
        animate["play"] = &Animation::PlaySequence;

        auto tileMap = lua.new_usertype<TileMap>("TileMap");
        tileMap["tiles"] = &TileMap::mTiles;

        auto entity = lua.new_usertype<Entity>("Entity");
        entity["getTransform"] = &Entity::getComponent<Transform>;
        entity["getTileMap"] = &Entity::getComponent<TileMap>;
        entity["getAnimate"] = &Entity::getComponent<Animation>;

        lua.set("input", Input::getInstance());
    }
}