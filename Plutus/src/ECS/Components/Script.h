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

        void update(float dt) {
            // mEnv["update"](dt); 
        }

    private:
        sol::environment mEnv;
    };
} // namespace Plutus class Script