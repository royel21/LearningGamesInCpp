#pragma once

#include <string>
#include <lua.hpp>
#include <iostream>
#include <sol/sol.hpp>

namespace Plutus
{
    class Script
    {
    public:
        std::string mScript;

    public:
        Script() = default;
        Script(const Script& script);
        Script(const std::string& script);

        void init(const std::string& _path = "");

        void update(float dt) {
            if (mEnv["update"] != sol::nil) {
                mEnv["update"](dt);
            }
        }

        void destroy();

    private:
        sol::environment mEnv;
    };
} // namespace Plutus class Script