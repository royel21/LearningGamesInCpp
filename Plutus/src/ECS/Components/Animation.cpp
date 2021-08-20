#include "Animation.h"

#include "ECS/Scene.h"
#include "Sprite.h"

namespace Plutus
{
    Sequence::Sequence(std::vector<int> frames, int texIndex, int _seqTime)
    {
        mFrames = frames;
        mTexIndex = texIndex;
        mSeqTime = _seqTime / 1000.0f;
    }

    void Animation::update(float dt)
    {
        if (mEnt->hasComponent<Sprite>() && mSequences.size())
        {
            auto &sprite = mEnt->getComponent<Sprite>();
            currentTime += dt;

            auto &seq = mSequences[currentSeq];
            auto &frames = seq.mFrames;

            if (currentTime > seq.mSeqTime)
            {
                seq.mFrame = ++seq.mFrame % frames.size();
                currentTime = 0;
            }

            sprite.mUVCoord = mTextures[seq.mTexIndex]->getUV(frames[seq.mFrame]);
        }
    }

    void Animation::addTexture(const std::string &id)
    {
        mTextures.push_back(Textures::get()->getTexture(id));
    }

    void Animation::AddSequence(const std::string id, Sequence seq)
    {
        mSequences[id] = seq;
    }

    void Animation::PlaySequence(const std::string &id, bool _loop)
    {
        currentSeq = id;
        loop = _loop;
    }

} // namespace Plutus