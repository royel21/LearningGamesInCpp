#pragma once
#include "System.h"
#include <sol/sol.hpp>

namespace Plutus
{
    class ScriptSystem : public ISystem
    {
    public:
        ScriptSystem(Scene* scene, Camera2D* camera);

        void init();
        void update(float dt);
        void destroy();
    private:
        sol::state mGlobalLua;
    };
} // namespace Plutus