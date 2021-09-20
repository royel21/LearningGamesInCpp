#include "Animation.h"

#include "ECS/Scene.h"
#include "Sprite.h"
#include <Assets/AssetManager.h>

namespace Plutus
{
    Sequence::Sequence(std::vector<int> frames, int texIndex, int _seqTime)
    {
        mFrames = frames;
        mTexIndex = texIndex;
        mSeqTime = _seqTime / 1000.0f;
    }

    void Animation::addTexture(const std::string& id)
    {
        mTextures.push_back(id);
    }

    void Animation::addSequence(const std::string id, Sequence seq)
    {
        mSequences[id] = seq;
    }

    void Animation::addSeq(const std::string id, std::vector<int> frames, int texIndex, int frameTime)
    {
        mSequences[id] = { frames, texIndex, frameTime };
    }

    void Animation::play(const std::string& id)
    {
        if (currentSeq != id && !loop) {
            currentSeq = id;
        }
    }

    Sequence* Animation::getCurrentSeq()
    {
        return &mSequences[currentSeq];
    }

} // namespace Plutus