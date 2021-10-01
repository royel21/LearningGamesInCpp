#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../Scene.h"

namespace Plutus
{
    class Entity;
    struct Texture;

    struct Sequence
    {
        float mSeqTime;
        int mTexIndex;
        uint32_t mFrame = 0;
        std::vector<int> mFrames;
        Sequence() = default;
        Sequence(std::vector<int> frames, int texIndex, int _seqTime = 100);
    };

    class Animation
    {
    public:
        Animation() = default;
        Animation(Entity& ent) : mEnt(ent) {};

        void init(Entity& ent) { mEnt = ent; };

        void addTexture(const std::string& id);
        void addSequence(const std::string id, Sequence seq);
        void addSeq(const std::string id, std::vector<int> frames, int texIndex, int frameTime);

        void setLoop(bool _loop) { loop = _loop; }
        void play(const std::string& id);

        Sequence* getCurrentSeq();

    public:
        std::string mState;
        std::string mPrevState;
        bool loop = false;
        bool animating = false;
        float currentTime = 0.0f;
        std::string currentSeq;
        std::unordered_map<std::string, Sequence> mSequences;
        std::vector<std::string> mTextures;
        Entity mEnt;
    };
} // namespace Plutus