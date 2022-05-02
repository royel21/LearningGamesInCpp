#pragma once
#include "System.h"
#include <sol/sol.hpp>

namespace Plutus
{
    class ScriptSystem : public ISystem
    {
    public:
        ScriptSystem(Camera2D* camera);

        void init(Project* project);
        void update(float dt);
    private:
        sol::state mGlobalLua;

        void registerAssets();
        void registerCamera();
        void registerEntity();
        void registerComponents();
    };
} // namespace Plutus