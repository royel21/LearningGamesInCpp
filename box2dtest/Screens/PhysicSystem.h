#pragma once
#include <Systems/Systems.h>
#include <box2d/box2d.h>
#include <memory>

namespace Plutus
{
    class PhysicSystem : ISystem
    {
    public:
        PhysicSystem(Scene* scene, Camera2D* camera);
        void update(float dt);

    private:
        std::unique_ptr<b2World> mWorld;
    };
} // namespace Plutus