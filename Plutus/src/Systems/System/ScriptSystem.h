#pragma once
#include "System.h"
#include <sol/sol.hpp>
#include <memory>

#include "EventListener/ICollisionListener.h"

namespace Plutus
{
    class ScriptSystem : public ISystem, public ICollisionListener
    {
    public:
        ScriptSystem(Camera2D* camera);

        void init();
        void update(float dt);

        void registerGlobal(std::function<void(sol::state&)> func) { func(mGlobalLua); };

        void CollisionEvent(uint32_t ent1, bool isSensorA, uint32_t ent2, bool isSensorB, bool collisionStart) override;

    private:
        sol::state mGlobalLua;
        uint32_t mEndId;

        void registerAssets();
        void registerCamera();
        void registerEntity();
        void registerComponents();
    };
} // namespace Plutus