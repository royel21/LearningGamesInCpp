#include "RenderSystem.h"

#include <Graphics/GLSL.h>
#include <Graphics/Camera2D.h>
#include <Graphics/FrameBuffer.h>

#include <ECS/Scene.h>
#include <ECS/Components/SpriteComponent.h>
#include <ECS/Components/TileMapComponent.h>
#include <ECS/Components/TransformComponent.h>

#include <Assets/Assets.h>

#include <Core/Project.h>

namespace Plutus
{
    RenderSystem::RenderSystem(Camera2D* camera) : ISystem(camera) {
        mRenderer.init();
    }

    void RenderSystem::init() {
        mRenderer.setCamera(mCamera);
    }

    void RenderSystem::update(float dt)
    {
        auto view = mProject->scene->getRegistry()->view<TransformComponent, SpriteComponent>();

        /******************Resize temp buffer************************/
        mRenderables.reserve(view.size_hint());
        // /******************************************/

        for (auto ent : view)
        {
            auto [trans, sprite] = view.get(ent);
            auto rect = trans.getRect();
            if (mCamera->getViewPortDim().overlap(rect))
            {
                auto tex = AssetManager::get()->getAsset<Texture>(sprite.mTextureId);
                mRenderables.emplace_back(tex, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY);
            }
        }
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());
    }

    void RenderSystem::draw() {
        mRenderer.submit(mRenderables);
        if (mFrameBuff) {
            mFrameBuff->bind();
            mRenderer.finish(BATCH_PICKING);
            mFrameBuff->unBind();
        }
        else {
            mRenderer.finish();
        }
        mRenderables.clear();
    }

    void RenderSystem::destroy()
    {
        mRenderables.clear();
    }

} // namespace Plutus