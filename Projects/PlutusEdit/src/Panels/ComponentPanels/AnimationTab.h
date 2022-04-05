#pragma once
#include <string>
#include <ECS/Components/AnimationComponent.h>

namespace Plutus
{
    enum Mode {
        NEW,
        EDIT
    };

    class Entity;
    class Textures;

    class AnimationTab
    {
    public:
        void DrawAnimation(Entity* mEnt);

    private:
        void SeqWindow();
    private:
        AnimationComponent* mAnimation;
        Textures* mTestures;
        std::string mCurSeq;
        std::string mCurTex;
        std::string mNewSeqId;
        Sequence* mCurrentSeq;
        Sequence newSeq;
        bool showSeqWindow = false;
        int mMode = NEW;
        float time = 0.0f;
    };
} // namespace Plutus