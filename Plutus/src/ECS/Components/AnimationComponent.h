#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#define Frames std::vector<uint32_t>

namespace Plutus
{

    class Entity;
    struct Texture;

    struct Sequence
    {
        std::string mTexId;
        float mSeqTime = 0.1f;
        uint32_t mFrame = 0;
        Frames mFrames;

        Sequence() = default;
        Sequence(const std::string& texId, Frames frames, int _seqTime = 100);
    };

    class AnimationComponent
    {
    public:
        AnimationComponent() = default;

        void addSequence(const std::string& id, const Sequence& seq);

        template<typename ...TArgs>
        inline void addSeq(TArgs ... args) {
            addSeq(std::forward<TArgs>(args)...);
        }

        void addSeq(const std::string& id, Frames frames, int frameTime = 0, bool isDefault = false);

        void setTexture(const std::string& texId);

        void updateSeq(const std::string& oldid, const std::string& newid);

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