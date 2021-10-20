#include "Animation.h"

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

    void Animation::addSequence(const std::string& id, Sequence seq)
    {
        mSequences[id] = seq;
    }

    void Animation::addSeq(const std::string& id, std::vector<int> frames, int texIndex, int frameTime)
    {
        mSequences[id] = { frames, texIndex, frameTime };
    }

    void Animation::replaceSeq(const std::string& oldid, const std::string& newid) {
        auto it = mSequences.find(oldid);
        if (it != mSequences.end()) {
            mSequences[newid] = mSequences[oldid];
            mSequences.erase(oldid);
        }
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