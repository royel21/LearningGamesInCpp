#include "RenderSystem.h"

#include <Graphics/GLSL.h>
#include <Graphics/Camera2D.h>

#include <ECS/Scene.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/Transform.h>


#include <Time/Timer.h>


namespace Plutus
{
    RenderSystem::RenderSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {
        mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);
        mRenderer.init();

        mDebug.init(camera);
    }

    void RenderSystem::update(float dt)
    {
        auto start = Timer::millis();

        auto viewMap = mScene->getRegistry()->view<TileMap>();
        auto view = mScene->getRegistry()->view<Transform, Sprite>();

        /******************Resize temp buffer************************/
        int size = view.size_hint();

        for (auto [ent, map] : viewMap.each()) {
            size += map.mTiles.size();
        }

        if (mRenderables.size() != size) {
            mRenderables.resize(size);
        }
        /******************************************/

        int i = 0;
        for (auto ent : viewMap)
        {
            auto [tilemap] = viewMap.get(ent);
            if (tilemap.mTiles.size())
            {
                auto tileset = tilemap.mTextures[0];
                const int w = tilemap.mTileWidth;
                const int h = tilemap.mTileHeight;

                for (auto& tile : tilemap.mTiles)
                {
                    auto tileset = tilemap.mTextures[tile.texture];
                    glm::vec4 rect{ tile.x, tile.y, w, h };
                    mRenderables[i++] = { tileset->texId, rect, tileset->getUV(tile.texcoord),
                        { tile.color }, tile.rotate, tile.flipX, tile.flipY, 0, tilemap.mLayer, false };
                }
            }
        }

        for (auto ent : view)
        {
            auto [trans, sprite] = view.get(ent);
            mRenderables[i++] = { sprite.getTexId(), trans.getRect(), sprite.mUVCoord, sprite.mColor,
                trans.r, sprite.mFlipX, sprite.mFlipY, entt::to_integral(ent), trans.layer, trans.sortY };

            mDebug.drawBox(trans.getRect(), {}, trans.r);
            mDebug.end();
            mDebug.render(2);

        }
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());

        mRenderer.submit(mRenderables);

        mRenderer.begin(&mShader, mCamera);
        mRenderer.draw();
        mRenderer.end();
        std::printf("time: %llu\n", Timer::millis() - start);
    }

    void RenderSystem::destroy()
    {
        mShader.dispose();
        mRenderables.clear();
    }

} // namespace Plutus