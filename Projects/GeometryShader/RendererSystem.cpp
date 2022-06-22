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

#include "TextComponent.h"

namespace Plutus
{
    void Layer::destroy() {
        for (auto& mbatch : tileMapBatchs) {
            mbatch->destroy();
            delete mbatch;
        }
        if (spriteBatch) {
            spriteBatch->destroy();
            delete spriteBatch;
        }
        if (textBatch) {
            textBatch->destroy();
            delete textBatch;
        }
    }

    Layer& RendererSystem::getLayer(uint32_t index) {
        if (index + 1 > mLayers.size()) {
            mLayers.resize(index + 1);
        }
        return mLayers[index];
    }

    SpriteBatch* RendererSystem::getSpriteBatch(uint32_t layerIndex) {
        Layer& layer = getLayer(layerIndex);

        if (!layer.spriteBatch) {
            layer.spriteBatch = new SpriteBatch(mCamera, &mSpriteShader);
            layer.spriteBatch->init();
        }

        return layer.spriteBatch;
    }

    SpriteBatch* RendererSystem::getTextBatch(uint32_t layerIndex) {
        Layer& layer = getLayer(layerIndex);
        if (!layer.textBatch) {
            layer.textBatch = new SpriteBatch(mCamera, &mTextShader);
            layer.textBatch->init();
        }

        return layer.textBatch;
    }

    void RendererSystem::init() {

        mTilemapShader.init(GLSLBatch::tmapVShader, GLSLBatch::tmapFShader);
        mSpriteShader.init(GLSLBatch::spriteVShader, GLSLBatch::spriteFShader);
        mTextShader.init(GLSLBatch::spriteVShader, GLSLBatch::textFrag);

        auto mapView = mProject->scene->getRegistry()->view<TileMapComponent>();
        for (auto [e, tmap] : mapView.each()) {
            Layer& layer = getLayer(tmap.mLayer);

            auto batchMap = new TileMapBatch(mCamera, &mTilemapShader);
            batchMap->init(&tmap, mProject);
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

        SpriteBatch* bath = nullptr;

        for (auto& r : mRenderables) {

            if (!bath) {
                bath = getSpriteBatch(r.layer);
            }
            //Add sprite to this layer
            bath->addSprite(&r);
        }

        auto textview = mProject->scene->getRegistry()->view<TransformComponent, TextComponent>();
        bath = nullptr;
        for (auto [e, trans, textc] : textview.each()) {

            if (!bath) {
                bath = getTextBatch(trans.layer);
            }
            auto pos = textc.mOffset + trans.getPosition();
            bath->addText(textc.mFontId, pos.x, pos.y, textc.mText, textc.mColor, textc.mScale);
        }

        for (auto& layer : mLayers) {
            for (auto mbatch : layer.tileMapBatchs) {
                if (mbatch) {
                    mbatch->update(dt);
                    mbatch->draw();
                }
            }

            if (layer.spriteBatch) {
                layer.spriteBatch->draw();
            }

            if (layer.textBatch) {
                layer.textBatch->draw();
            }
        }
    }

    void RendererSystem::destroy() {
        for (auto& layer : mLayers) {
            layer.destroy();
        }

        mLayers.clear();
        mTextShader.destroy();
        mTilemapShader.destroy();
        mSpriteShader.destroy();
    }
}