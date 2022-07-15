#include "AnimationComponent.h"

namespace Plutus
{
    Sequence::Sequence(const std::string& texId, Frames frames, int _seqTime)
    {
        mTexId = texId;
        mFrames = frames;
        mSeqTime = _seqTime / 1000.0f;
    }

    void AnimationComponent::addSequence(const std::string& id, const Sequence& seq)
    {
        if (!id.empty())
            mSequences[id] = seq;
    }

    void AnimationComponent::addSeq(const std::string& id, Frames frames, int frameTime, bool isDefault)
    {
        if (!id.empty()) {
            mSequences[id] = { tempSprite, frames, frameTime };

            if (isDefault)
                currentSeq = id;
        }
    }

    void AnimationComponent::setTexture(const std::string& texId)
    {
        tempSprite = texId;
    }

    void AnimationComponent::updateSeq(const std::string& oldid, const std::string& newid) {
        if (oldid == newid) return;

        auto it = mSequences.find(oldid);
        if (it != mSequences.end()) {
            mSequences[newid] = mSequences[oldid];
            mSequences.erase(oldid);
        }
    }

    void AnimationComponent::play(const std::string& id)
    {
        if (currentSeq != id && !loop) {
            currentSeq = id;
        }
    }

    Sequence* AnimationComponent::getCurrentSeq()
    {
        if (mSequences.size()) {
            auto it = mSequences.find(currentSeq);
            if (it != mSequences.end()) {
                return &it->second;
            }
            else {
                return &mSequences.begin()->second;
            }
        }
        return nullptr;
    }

} // namespace Plutus