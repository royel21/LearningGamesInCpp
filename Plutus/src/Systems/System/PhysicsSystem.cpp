#include "PhysicsSystem.h"
#include <ECS/Scene.h>
#include <ECS/Components.h>

namespace Plutus
{
    PhysicsSystem::PhysicsSystem(Scene* scene, vec2f gravity) : ISystem(scene) {
        mScene->getWorld()->SetGravity(b2Vec2{ gravity.x, gravity.y });
    }

    void PhysicsSystem::init() {
        // auto view = mScene->getRegistry()->view<RigidBody>();
    }

    void PhysicsSystem::update(float dt) {

    }

    void PhysicsSystem::destroy() {
        mScene->resetWorld();
    }
} // namespace Plutus

