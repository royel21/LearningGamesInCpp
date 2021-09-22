#include "PhysicSystem.h"

namespace Plutus {


    PhysicSystem::PhysicSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {
        b2Vec2 gravity(0.0f, -9.81f);
        mWorld = std::make_unique<b2World>(gravity);
    }

    void PhysicSystem::update(float dt)
    {

    }


}