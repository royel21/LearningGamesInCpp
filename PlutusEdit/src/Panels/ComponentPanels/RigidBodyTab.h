#pragma once

namespace Plutus
{
    class Entity;
    class RigidBody;

    class RigidBodyTab
    {
    public:
        RigidBodyTab();
        void draw(Entity* ent);

    private:
        RigidBody* mRigidBody;
    };
} // namespace Plutus