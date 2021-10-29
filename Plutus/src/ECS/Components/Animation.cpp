#include "Animation.h"

namespace Plutus
{
    Sequence::Sequence(const std::string& texId, Frames frames, int _seqTime)
    {
        mTexId = texId;
        mFrames = frames;
        mSeqTime = _seqTime / 1000.0f;
    }

    void Animation::addSequence(const std::string& id, const Sequence& seq)
    {
        if (!id.empty())
            mSequences[id] = seq;
    }

    void Animation::addSeq(const std::string& id, Frames frames, int frameTime, bool isDefault)
    {
        if (!id.empty()) {
            mSequences[id] = { tempSprite, frames, frameTime };

            if (isDefault)
                currentSeq = id;
        }
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
        else if (mSequences.size()) {
            return &mSequences.begin()->second;
        }
        return nullptr;
    }

} // namespace Plutus