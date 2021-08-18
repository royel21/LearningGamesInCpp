#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <string>
#include <iostream>
#include "ECS/Scene.h"

namespace Plutus
{
    class Entity;

    class Script
    {
    public:
        std::string path;

    public:
        Script(std::string _path, Entity* ent, Scene* scene);

        void update(float dt) { lua["update"](dt); }
        inline sol::state* getState() { return &lua; };

    private:
        sol::state lua;
    };

    inline void my_panic(sol::optional<std::string> maybe_msg)
    {
        std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
        if (maybe_msg)
        {
            const std::string& msg = maybe_msg.value();
            std::cerr << "\terror message: " << msg << std::endl;
        }
        // When this function exits, Lua will exhibit default behavior and abort()
    }
} // namespace Plutus class Script