#pragma once
#include "System.h"
#include <sol/sol.hpp>
#include <Math/Vectors.h>
#include "EventListener/ICollisionListener.h"

namespace Plutus
{
    class PhysicSystem;

    class ScriptSystem : public ISystem, public ICollisionListener
    {
    public:
        ScriptSystem(Camera2D* camera);

        void init() override;
        void update(float dt) override;

        void registerGlobal(std::function<void(sol::state&)> func) { func(mGlobalLua); };

        void CollisionEvent(uint32_t ent1, bool isSensorA, uint32_t ent2, bool isSensorB, bool collisionStart) override;

    private:
        sol::state mGlobalLua;
        PhysicSystem* mPhysicSys;
        void registerAssets();
        void registerCamera();
        void registerEntity();
        void registerComponents();
        void registerGlobals();
        void registerPhysics();
        uint32_t castRay(const Vec2f& start, const Vec2f& end, uint32_t mask = 0xffff);
    };
} // namespace Plutus