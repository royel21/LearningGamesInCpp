#pragma once

namespace Plutus
{
    class Entity;
    struct RigidBodyComponent;
    struct Fixture;

    class RigidBodyTab
    {
    public:
        RigidBodyTab();
        void draw(Entity* ent);

    private:
        RigidBodyComponent* mRigidBody;
        float mTextRowWidth = 0.3f;
    };
} // namespace Plutus