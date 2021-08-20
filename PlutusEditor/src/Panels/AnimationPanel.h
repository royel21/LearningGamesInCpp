#pragma once
#include "ECS/Components/Animation.h"

namespace Plutus
{
    class Entiry;
    class AnimationPanel
    {
        Sequence mSeq;
        char name[128] = {};

    public:
        void draw(Entity *ent);

    private:
        void SequenceEditor(bool &show);
    };
}