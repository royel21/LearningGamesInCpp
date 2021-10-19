#pragma once

#include <string>
#include <lua.hpp>
#include <iostream>
#include <sol/sol.hpp>

#include "../Scene.h"

namespace Plutus
{
    class Script
    {
    public:
        Entity ent;
        Scene* scene;
        std::string path;

    public:
        Script() = default;
        Script(const Script& script);

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