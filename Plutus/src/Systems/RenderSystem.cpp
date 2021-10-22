#include "RenderSystem.h"

#include <Graphics/GLSL.h>
#include <Graphics/Camera2D.h>

#include <ECS/Scene.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/Transform.h>

#include <Assets/AssetManager.h>

namespace Plutus
{
    RenderSystem::RenderSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {
        mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);
        mRenderer.init();
        mRenderer.setShader(&mShader);
        mRenderer.setCamera(camera);
    }

    RenderSystem::~RenderSystem()
    {
        mShader.dispose();
    }

    void RenderSystem::update(float dt)
    {
        // auto start = Timer::millis();

        auto viewMap = mScene->getRegistry()->view<TileMap>();
        auto view = mScene->getRegistry()->view<Transform, Sprite>();

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
                const int w = tilemap.mTileWidth;
                const int h = tilemap.mTileHeight;

                for (auto& tile : tilemap.mTiles)
                {
                    glm::vec4 rect = tile.getRect();
                    if (mCamera->isBoxInView(rect, 200))
                    {
                        auto tex = tilemap.getTexture(tile.texture);
                        if (tex) {
                            mRenderables[i++] = { tex->texId, rect, tex->getUV(tile.texcoord),
                                { tile.color }, tile.rotate, tile.flipX, tile.flipY, 0, tilemap.mLayer, false };
                        }
                    }
                }
            }
        }

        for (auto ent : view)
        {
            auto [trans, sprite] = view.get(ent);
            auto rect = trans.getRect();
            if (mCamera->isBoxInView(rect, 200))
            {
                auto texId = AssetManager::get()->getTexId(sprite.mTextureId);
                mRenderables[i++] = { texId, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY };
            }

        }
        mRenderables.resize(i);
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());
        mRenderer.submit(mRenderables);

        mRenderer.begin();
        mRenderer.draw();
        mRenderer.end();
    }

    void RenderSystem::destroy()
    {
        mRenderables.clear();
    }

} // namespace Plutus