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
        mShader.init(GLSL::vertexShader, GLSL::fragShader);
        mRenderer.init();
    }

    void RenderSystem::init(Project* project) {
        mProject = project;
        mRenderer.setShader(&mShader);
        mRenderer.setCamera(mCamera);
    }

    void RenderSystem::update(float dt)
    {
        // auto start = Timer::millis();

        auto viewMap = mProject->scene->getRegistry()->view<TileMapComponent>();
        auto view = mProject->scene->getRegistry()->view<TransformComponent, SpriteComponent>();

        /******************Resize temp buffer************************/
        auto size = view.size_hint();

        for (auto [ent, map] : viewMap.each()) {
            size += map.mTiles.size();
        }
        mRenderables.resize(size);

        /******************************************/

        int i = 0;
        for (auto ent : viewMap)
        {
            auto [tilemap] = viewMap.get(ent);
            if (tilemap.mTiles.size())
            {
                ColorRGBA8 color;
                for (auto& tile : tilemap.mTiles)
                {
                    auto rect = tilemap.getRect(tile);
                    if (mCamera->isBoxInView(rect))
                    {
                        auto texIndex = -1;
                        Texture* tex = nullptr;
                        uint32_t texId;

                        if (texIndex != tile.texture) {
                            tex = tilemap.getTexture(tile.texture);
                            texId = tex ? tex->mTexId : -1;
                        }

                        if (tex) {
                            mRenderables[i++] = {
                                tex->mTexId, // Texure Id
                                rect, // Desct Rectangle
                                tex->getUV(tile.texcoord), // Texure Coords UV
                                color, // Color
                                tile.rotate, // Rotation
                                tile.flipX, // Flip X
                                tile.flipY, // Flip Y
                                0, // Entity Id
                                tilemap.mLayer// Layer in which to be draw
                            };
                        }
                    }
                }
            }
        }

        for (auto ent : view)
        {
            auto [trans, sprite] = view.get(ent);
            auto rect = trans.getRect();
            if (mCamera->isBoxInView(rect))
            {
                auto tex = AssetManager::get()->getAsset<Texture>(sprite.mTextureId);
                mRenderables[i++] = { tex ? tex->mTexId : 0, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY };
            }
        }
        mRenderables.resize(i);
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());
        mRenderer.submit(mRenderables);
        if (mFrameBuff) {
            mFrameBuff->bind();
            mRenderer.finish(BATCH_PICKING);
            mFrameBuff->unBind();
        }
        else {
            mRenderer.finish();
        }

    }

    void RenderSystem::destroy()
    {
        mRenderables.clear();
    }

} // namespace Plutus