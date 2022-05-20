#include "RendererSystem.h"
#include <algorithm>

#include <ECS/Scene.h>
#include <Core/Project.h>
#include <ECS/Components.h>

#include "GLSLBatch.h"
#include "TileMapBatch.h"
#include "SpriteBatch.h"


namespace Plutus
{
    void RendererSystem::init(Project* project) {
        mProject = project;

        mTilemapShader.init(GLSLBatch::tmapVShader, GLSLBatch::tmapFShader);

        auto mapView = mProject->scene->getRegistry()->view<TileMapComponent>();
        for (auto [e, tmap] : mapView.each()) {
            if (tmap.mLayer + 1 > (int)mLayers.size()) {
                mLayers.resize(tmap.mLayer + 1);
            }
            Layer& layer = mLayers[tmap.mLayer];

            auto batchMap = new TileMapBatch(mCamera, &mTilemapShader);
            batchMap->init(&tmap);
            layer.tileMapBatchs.push_back(batchMap);
        }
    }

    void RendererSystem::update(float dt) {

        auto view = mProject->scene->getRegistry()->view<TransformComponent, SpriteComponent>();

        for (auto& layer : mLayers) {
            for (auto mbatch : layer.tileMapBatchs) {
                if (mbatch) {
                    mbatch->draw();
                }
            }
            if (layer.spriteBatch)
                layer.spriteBatch->draw();
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
    }
}