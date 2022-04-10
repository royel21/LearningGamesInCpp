#pragma once

namespace Plutus
{
    class Entity;
    class RigidBodyComponent;
    struct Fixture;

    class RigidBodyTab
    {
    public:
        RigidBodyTab();
        void draw(Entity* ent);

    private:
        bool drawFixture(Fixture& fix, int index);

    private:
        RigidBodyComponent* mRigidBody;
        float mTextRowWidth = 0.3f;
    };
} // namespace Plutus