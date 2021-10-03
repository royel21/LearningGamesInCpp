#pragma once
#include <sol/sol.hpp>

namespace Plutus
{
    class Scene;

    class ScriptServer
    {
    public:
        static ScriptServer* get();
        void setScene(Scene* scene);
        sol::state* getState() { return &lua; }

    private:
        ScriptServer();
        sol::state lua;
    };
} // namespace Plutus