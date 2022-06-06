#include "RendererSystem.h"
#include <algorithm>

#include <ECS/Scene.h>
#include <Core/Project.h>
#include <Assets/Assets.h>
#include <ECS/Components.h>
#include <Graphics/Camera2D.h>

#include "GLSLBatch.h"
#include "TileMapBatch.h"
#include "SpriteBatch.h"

namespace Plutus
{
    void RendererSystem::init() {

        mTilemapShader.init(GLSLBatch::tmapVShader, GLSLBatch::tmapFShader);
        mSpriteShader.init(GLSLBatch::spriteVShader, GLSLBatch::spriteFShader);

        auto mapView = mProject->scene->getRegistry()->view<TileMapComponent>();
        for (auto [e, tmap] : mapView.each()) {
            Layer& layer = getLayer(tmap.mLayer);

            auto batchMap = new TileMapBatch(mCamera, &mTilemapShader);
            batchMap->init(&tmap);
            layer.tileMapBatchs.push_back(batchMap);
        }
    }

    void RendererSystem::update(float dt) {

        auto view = mProject->scene->getRegistry()->view<TransformComponent, SpriteComponent>();
        mRenderables.clear();

        mRenderables.reserve(view.size_hint());

        //Get all visible sprite and create renderables
        for (auto [e, trans, sprite] : view.each()) {
            auto rect = trans.getRect();
            if (mCamera->getViewPortDim().overlap(rect)) {
                auto tex = AssetManager::get()->getAsset<Texture>(sprite.mTextureId);
                mRenderables.emplace_back(tex, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(e), trans.layer, trans.sortY, trans.offsetY);
            }
        }

        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());

        for (auto& r : mRenderables) {
            Layer& layer = getLayer(r.layer + 1);

            if (!layer.spriteBatch) {
                layer.spriteBatch = new SpriteBatch(mCamera, &mSpriteShader);
                layer.spriteBatch->init();
            }

            //Add sprite to this layer
            layer.spriteBatch->addSprite(&r);
        }

        for (auto& layer : mLayers) {
            for (auto mbatch : layer.tileMapBatchs) {
                if (mbatch) {
                    mbatch->draw();
                }
            }
            if (layer.spriteBatch) {
                layer.spriteBatch->draw();
            }
        }
    }

    void RendererSystem::destroy() {
        for (auto& layer : mLayers) {
            for (auto& mbatch : layer.tileMapBatchs) {
                mbatch->destroy();
                delete mbatch;
            }
            if (layer.spriteBatch) {
                delete layer.spriteBatch;
            }
        }
        mLayers.clear();
        mTilemapShader.destroy();
        mSpriteShader.destroy();
    }
}