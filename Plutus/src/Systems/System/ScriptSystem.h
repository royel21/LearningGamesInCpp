#pragma once
#include "System.h"
#include <sol/sol.hpp>
#include <memory>

namespace Plutus
{
    class ScriptSystem : public ISystem
    {
    public:
        ScriptSystem(Camera2D* camera);

        void init(Project* project);
        void update(float dt);

        void registerGlobal(std::function<void(sol::state&)> func) { func(mGlobalLua); };

    private:
        sol::state mGlobalLua;

        void registerAssets();
        void registerCamera();
        void registerEntity();
        void registerComponents();
    };
} // namespace Plutus