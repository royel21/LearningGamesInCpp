#pragma once

#include <string>
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

        void setScript(const std::string& script);

        void init(sol::state& lua);

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