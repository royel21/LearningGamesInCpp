#pragma once

namespace Plutus
{
    class Entity;
    class Textures;
    class AnimationTab
    {
    public:
        void DrawAnimation(Entity* mEnt);
    private:
        Entity* mEnt;
        Textures* mTestures;
    };
} // namespace Plutus