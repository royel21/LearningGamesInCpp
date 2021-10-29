#include "PhysicSystem.h"
#include <ECS/Scene.h>
#include <ECS/Components.h>

namespace Plutus
{
    PhysicSystem::PhysicSystem(Scene* scene, vec2f gravity) : ISystem(scene) {
        mScene->getWorld()->SetGravity(b2Vec2{ gravity.x, gravity.y });
    }

    void PhysicSystem::init() {
        // auto view = mScene->getRegistry()->view<RigidBody>();
    }

    void PhysicSystem::update(float dt) {
        mScene->updateWorld();


    }



    void PhysicSystem::destroy() {
        mScene->resetWorld();
    }
} // namespace Plutus

