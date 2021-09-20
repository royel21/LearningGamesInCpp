#include "RenderSystem.h"
#include "Graphics/GLSL.h"
#include <ECS/Components.h>

#include <ECS/Scene.h>
#include <Graphics/Camera2D.h>


namespace Plutus
{
    RenderSystem::RenderSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {
        mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);
        mRenderer.init();
    }

    void RenderSystem::update(float dt)
    {
        auto mLayers = mScene->getLayers();
        for (auto& layer : *mLayers)
        {
            if (layer.second.isVisible)
            {
                for (auto ent : layer.second.mEntities)
                {
                    if (ent->hasComponent<TileMap>())
                    {
                        auto& map = ent->getComponent<TileMap>();
                        auto tileset = map.mTextures[0];
                        const int w = map.mTileWidth;
                        const int h = map.mTileHeight;

                        if (map.mTiles.size())
                        {
                            std::sort(map.mTiles.begin(), map.mTiles.end(), [](Tile& t1, Tile& t2) -> bool
                                { return t1.texture < t2.texture; });

                            mRenderer.reserve(map.mTiles.size());
                            for (auto& tile : map.mTiles)
                            {
                                auto tileset = map.mTextures[tile.texture];
                                glm::vec4 rect{ tile.x, tile.y, w, h };
                                mRenderer.submit(tileset->texId, rect, tileset->getUV(tile.texcoord), { tile.color }, tile.rotate, tile.flipX, tile.flipY);
                            }
                        }
                    }

                    if (ent->hasComponent<Sprite>())
                    {
                        auto& trans = ent->getComponent<Transform>();
                        auto& sprite = ent->getComponent<Sprite>();
                        mRenderer.submit(sprite.getTexId(), trans.getRect(), sprite.mUVCoord, sprite.mColor, trans.r, sprite.mFlipX, sprite.mFlipY);
                    }
                }
            }
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