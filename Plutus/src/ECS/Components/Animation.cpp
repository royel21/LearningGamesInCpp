#include "Animation.h"

namespace Plutus
{
    Sequence::Sequence(const std::string& texId, std::vector<int> frames, int _seqTime)
    {
        mTexId = texId;
        mFrames = frames;
        mSeqTime = _seqTime / 1000.0f;
    }

    void Animation::addSequence(const std::string& id, const Sequence& seq)
    {
        mSequences[id] = seq;
    }

    void Animation::addSeq(const std::string& id, std::vector<int> frames, int frameTime)
    {
        mSequences[id] = { tempSprite, frames, frameTime };
    }

    void Animation::setTexture(const std::string& texId)
    {
        tempSprite = texId;
    }

    void Animation::swapSeq(const std::string& oldid, const std::string& newid) {
        if (oldid == newid) return;

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
        auto it = mSequences.find(currentSeq);
        if (it != mSequences.end()) {
            return &it->second;
        }
        return nullptr;
    }

} // namespace Plutus