#pragma once

namespace Plutus
{
    class Entity;
    class RigidBodyComponent;

    class RigidBodyTab
    {
    public:
        RigidBodyTab();
        void draw(Entity* ent);

    private:
        RigidBodyComponent* mRigidBody;
    };
} // namespace Plutus