#include "AnimationSystem.h"

#include <ECS/Scene.h>
#include <ECS/Components/Sprite.h> 
#include <ECS/Components/Animation.h>

#include <Assets/AssetManager.h>

namespace Plutus
{
    void AnimationSystem::update(float dt)
    {
        auto view = mScene->getRegistry()->view<Sprite, Animation>();
        for (auto ent : view)
        {
            auto [sprite, animation] = view.get(ent);
            auto seq = animation.getCurrentSeq();
            if (seq && seq->mFrames.size()) {
                animation.currentTime += dt;
                auto& frames = seq->mFrames;

                if (animation.currentTime > seq->mSeqTime)
                {
                    seq->mFrame = ++seq->mFrame % frames.size();
                    animation.currentTime = 0;
                    if (seq->mFrame + 1 == frames.size() && animation.loop) {
                        animation.loop = false;
                        seq->mFrame = 0;
                        animation.currentTime = 0;
                    }
                }

                sprite.mTextureId = seq->mTexId;
                sprite.mUVCoord = AssetManager::get()->getTexCoords(seq->mTexId, frames[seq->mFrame]);
            }
        }
    }

}