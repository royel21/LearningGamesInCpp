#include "Animate.h"

#include "ECS/Scene.h"
#include "Sprite.h"

namespace Plutus
{
    Animation::Animation(std::vector<int> frames, int texIndex, int _animTime)
    {
        mFrames = frames;
        mTexIndex = texIndex;
        mAnimTime = _animTime / 1000.0f;
    }

    void Animate::update(float dt)
    {
        if (mEnt->hasComponent<Sprite>() && animations.size())
        {
            auto &sprite = mEnt->getComponent<Sprite>();
            currentTime += dt;

            auto &anim = animations[currentAnin];
            auto &frames = anim.mFrames;

            if (currentTime > anim.mAnimTime)
            {
                anim.mFrame = ++anim.mFrame % frames.size();
                currentTime = 0;
            }

            sprite.mUVCoord = mTextures[anim.mTexIndex]->getUV(frames[anim.mFrame]);
        }
    }

    void Animate::addTexture(const std::string &id)
    {
        mTextures.push_back(Textures::get()->getTexture(id));
    }

    void Animate::AddAnimation(const std::string id, Animation anin)
    {
        animations[id] = anin;
    }

    void Animate::PlayAnimation(const std::string &id, bool _loop)
    {
        currentAnin = id;
        loop = _loop;
    }

} // namespace Plutus