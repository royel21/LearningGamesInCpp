#pragma once

#include <vector>
#include <string>
#include <unordered_map>

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
        Animation(Entity* ent) : mEnt(ent) {};

        void update(float dt);
        void addTexture(const std::string& id);
        void AddSequence(const std::string id, Sequence seq);
        void PlaySequence(const std::string& id, bool _loop = false);

    public:
        bool loop = false;
        float currentTime = 0.0f;
        std::string currentSeq;
        std::unordered_map<std::string, Sequence> mSequences;
        std::vector<Texture*> mTextures;
        Entity* mEnt = nullptr;
    };
} // namespace Plutus