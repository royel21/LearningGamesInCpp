#pragma once
#include <sol/sol.hpp>

namespace Plutus
{
    class ScriptServer
    {
    public:
        static ScriptServer* get();
        sol::state* getState() { return &lua; }

    private:
        ScriptServer();
        sol::state lua;
    };
} // namespace Plutus