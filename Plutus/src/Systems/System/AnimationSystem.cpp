#include "AnimationSystem.h"

#include <ECS/Scene.h>
#include <ECS/Components/SpriteComponent.h> 
#include <ECS/Components/ScriptComponent.h>
#include <ECS/Components/AnimationComponent.h>

#include <Assets/Assets.h>
#include <Core/Project.h>

namespace Plutus
{
    void AnimationSystem::init() {
        auto view = mProject->scene->getRegistry()->view<SpriteComponent, AnimationComponent>();
        for (auto ent : view)
        {
            auto [sprite, animation] = view.get(ent);
            auto seq = animation.getCurrentSeq();

            if (!seq && animation.mSequences.size()) seq = &animation.mSequences.begin()->second;

            if (seq && seq->mFrames.size() && !seq->mTexId.empty()) {
                sprite.mTextureId = seq->mTexId;
                sprite.mUVCoord = AssetManager::get()->getAsset<Texture>(seq->mTexId)->getUV(seq->mFrames[0]);
            }
        }
    }

    void AnimationSystem::update(float dt)
    {
        auto view = mProject->scene->getRegistry()->view<SpriteComponent, AnimationComponent>();
        for (auto ent : view)
        {
            auto [sprite, animation] = view.get(ent);
            auto seq = animation.getCurrentSeq();

            if (seq && !seq->mTexId.empty() && seq->mFrames.size()) {
                auto framesCount = seq->mFrames.size();

                animation.currentTime += dt;

                if (animation.currentTime > seq->mSeqTime)
                {
                    seq->mFrame = ++seq->mFrame % framesCount;
                    if (framesCount > 1 && seq->mFrame + 1 == framesCount) {

                        if (animation.loop) {
                            animation.loop = false;
                            seq->mFrame = 0;
                        }

                        auto script = mProject->scene->getComponent<ScriptComponent>(ent);
                        if (script) {
                            script->animationEnd(animation.currentSeq);
                        }
                    }
                    animation.currentTime = 0;
                }

                sprite.mTextureId = seq->mTexId;
                sprite.mUVCoord = AssetManager::get()->getAsset<Texture>(seq->mTexId)->getUV(seq->mFrames[seq->mFrame]);
            }
        }
    }

}