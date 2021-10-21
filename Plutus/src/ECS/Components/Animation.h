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
        std::string mTexId;
        float mSeqTime = 0.1f;
        uint32_t mFrame = 0;
        std::vector<int> mFrames;

        Sequence() = default;
        Sequence(const std::string& texId, std::vector<int> frames, int _seqTime = 100);
    };

    class Animation
    {
    public:
        Animation() = default;
        void addSequence(const std::string& id, const Sequence& seq);
        void addSeq(const std::string& id, std::vector<int> frames, int frameTime);

        void setTexture(const std::string& texId);

        void swapSeq(const std::string& oldid, const std::string& newid);

        void setLoop(bool _loop) { loop = _loop; }
        void play(const std::string& id);

        Sequence* getCurrentSeq();

    public:
        bool loop = false;
        bool animating = false;
        float currentTime = 0.0f;
        std::string tempSprite;
        std::string currentSeq;
        std::unordered_map<std::string, Sequence> mSequences;
    };
} // namespace Plutus