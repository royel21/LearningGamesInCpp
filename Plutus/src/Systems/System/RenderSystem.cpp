#include "RenderSystem.h"

#include <Graphics/GLSL.h>
#include <Graphics/Camera2D.h>

#include <ECS/Scene.h>
#include <ECS/Components/SpriteComponent.h>
#include <ECS/Components/TileMapComponent.h>
#include <ECS/Components/TransformComponent.h>

#include <Assets/AssetManager.h>

namespace Plutus
{
    RenderSystem::RenderSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {
        mShader.init(GLSL::vertexShader, GLSL::fragShader);
        mRenderer.init();
        mRenderer.setShader(&mShader);
        mRenderer.setCamera(camera);
    }

    RenderSystem::~RenderSystem()
    {
        mShader.destroy();
    }

    void RenderSystem::update(float dt)
    {
        // auto start = Timer::millis();

        auto viewMap = mScene->getRegistry()->view<TileMapComponent>();
        auto view = mScene->getRegistry()->view<TransformComponent, SpriteComponent>();

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
                for (auto& tile : tilemap.mTiles)
                {
                    auto rect = tile.getRect();
                    if (mCamera->isBoxInView(rect, 200))
                    {
                        auto tex = tilemap.getTexture(tile.texture);
                        if (tex) {
                            mRenderables[i++] = {
                                tex->texId, // Texure Id
                                rect, // Desct Rectangle
                                tex->getUV(tile.texcoord), // Texure Coords UV
                                { tile.color }, // Color
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
            if (mCamera->isBoxInView(rect, 200))
            {
                auto texId = AssetManager::get()->getTexId(sprite.mTextureId);
                mRenderables[i++] = { texId, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY };
            }

        }
        mRenderables.resize(i);
        // sort by layer, y position, texture
        // std::sort(mRenderables.begin(), mRenderables.end());
        mRenderer.submit(mRenderables);

        mRenderer.finish();
    }

    void RenderSystem::destroy()
    {
        mRenderables.clear();
    }

} // namespace Plutus