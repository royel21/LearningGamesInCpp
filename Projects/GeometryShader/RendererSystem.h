#pragma once
#include <cstdint>

#include <vector>

#include <Graphics/Shader.h>
#include <Systems/System/System.h>

#include <vector>

#include <Graphics/Renderables.h>

namespace Plutus
{
    class TileMapBatch;
    class SpriteBatch;

    struct Layer {
        std::vector<TileMapBatch*> tileMapBatchs;
        SpriteBatch* spriteBatch = nullptr;
        SpriteBatch* textBatch = nullptr;
        Layer() {};
        void destroy();
    };

    class RendererSystem : public ISystem
    {
    public:
        RendererSystem(Camera2D* camera) : ISystem(camera) {};

        void init() override;
        void update(float dt) override;
        void destroy() override;

    private:
        Shader mTilemapShader;
        Shader mSpriteShader;
        Shader mTextShader;
        std::vector<Layer> mLayers;
        std::vector<Renderable> mRenderables;

        SpriteBatch* getTextBatch(uint32_t layer);
        SpriteBatch* getSpriteBatch(uint32_t layer);
        Layer& getLayer(uint32_t index);
    };
} // namespace Plutus