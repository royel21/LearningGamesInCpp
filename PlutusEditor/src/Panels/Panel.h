#pragma once

namespace Plutus
{
    class Entity;
    class Panel
    {
    public:
        void init(Entity* ent) { mEnt = ent; };
        virtual void drawPanel() = 0;
        virtual void showCreate() = 0;
    public:
        Entity* mEnt;
    };
} // namespace Plutus