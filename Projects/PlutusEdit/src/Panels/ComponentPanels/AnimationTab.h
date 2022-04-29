#pragma once
#include <string>
#include <ECS/Components/AnimationComponent.h>

namespace Plutus
{
    struct Config;

    enum Mode {
        NEW,
        EDIT
    };

    class Entity;

    class AnimationTab
    {
    public:
        void draw(Config* config);

    private:
        void SeqWindow();
    private:
        AnimationComponent* mAnimation;
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