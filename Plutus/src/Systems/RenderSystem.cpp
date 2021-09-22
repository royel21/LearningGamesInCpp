#include "RenderSystem.h"

#include <Graphics/GLSL.h>
#include <Graphics/Camera2D.h>

#include <ECS/Scene.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/Transform.h>



namespace Plutus
{
    RenderSystem::RenderSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {
        mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);
        mRenderer.init();
    }

    void RenderSystem::update(float dt)
    {
        // auto mLayers = mScene->getLayers();
        // for (auto& layer : *mLayers)
        // {
        //     if (layer.second.isVisible)
        //     {
        //         for (auto ent : layer.second.mEntities)
        //         {
        //             if (ent->hasComponent<TileMap>())
        //             {
        //                 auto& map = ent->getComponent<TileMap>();
        //                 auto tileset = map.mTextures[0];
        //                 const int w = map.mTileWidth;
        //                 const int h = map.mTileHeight;

        //                 if (map.mTiles.size())
        //                 {
        //                     std::sort(map.mTiles.begin(), map.mTiles.end(), [](Tile& t1, Tile& t2) -> bool
        //                         { return t1.texture < t2.texture; });

        //                     mRenderer.reserve(map.mTiles.size());
        //                     for (auto& tile : map.mTiles)
        //                     {
        //                         auto tileset = map.mTextures[tile.texture];
        //                         glm::vec4 rect{ tile.x, tile.y, w, h };
        //                         mRenderer.submit(tileset->texId, rect, tileset->getUV(tile.texcoord), { tile.color }, tile.rotate, tile.flipX, tile.flipY);
        //                     }
        //                 }
        //             }

        //             if (ent->hasComponent<Sprite>())
        //             {
        //                 auto& trans = ent->getComponent<Transform>();
        //                 auto& sprite = ent->getComponent<Sprite>();
        //                 mRenderer.submit(sprite.getTexId(), trans.getRect(), sprite.mUVCoord, sprite.mColor, trans.r, sprite.mFlipX, sprite.mFlipY);
        //             }
        //         }
        //     }
        // }
        auto viewMap = mScene->getRegistry()->view<TileMap>();
        for (auto ent : viewMap)
        {
            auto [tilemap] = viewMap.get(ent);
            if (tilemap.mTiles.size())
            {
                auto tileset = tilemap.mTextures[0];
                const int w = tilemap.mTileWidth;
                const int h = tilemap.mTileHeight;

                mRenderer.reserve(tilemap.mTiles.size());
                for (auto& tile : tilemap.mTiles)
                {
                    auto tileset = tilemap.mTextures[tile.texture];
                    glm::vec4 rect{ tile.x, tile.y, w, h };
                    // mRenderer.submit(tileset->texId, rect, tileset->getUV(tile.texcoord), { tile.color }, tile.rotate, tile.flipX, tile.flipY);
                    mRenderer.submitRenderable(tileset->texId, rect, tileset->getUV(tile.texcoord), { tile.color }, tile.rotate, tile.flipX, tile.flipY, 0, tilemap.layer, false);
                }
            }
        }

        auto view = mScene->getRegistry()->view<Transform, Sprite>();
        for (auto ent : view)
        {

            auto [trans, sprite] = view.get(ent);
            mRenderer.submitRenderable(sprite.getTexId(), trans.getRect(), sprite.mUVCoord, sprite.mColor, trans.r, sprite.mFlipX, sprite.mFlipY, entt::to_integral(ent), trans.layer, trans.sortY);
        }

        mRenderer.begin(&mShader, mCamera);
        mRenderer.draw();
        mRenderer.end();
    }

    void RenderSystem::destroy()
    {
        mShader.dispose();
    }

} // namespace Plutus