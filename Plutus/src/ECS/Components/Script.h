#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <string>
#include <iostream>
#include "ECS/Scene.h"

namespace Plutus
{
    class Script
    {
    public:
        Scene* scene;
        Entity ent;
        std::string path;

    public:
        Script() = default;
        Script(const Script& script);
        Script(const std::string&, Entity ent, Scene* scene);

        void init(const std::string& _path, Entity _ent, Scene* _scene);

        void update(float dt) {
            if (mEnv["update"] != sol::nil) {
                mEnv["update"](dt);
            }
        }

    private:
        sol::environment mEnv;
    };
} // namespace Plutus class Script