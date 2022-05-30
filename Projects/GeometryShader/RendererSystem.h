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
        Layer() {};
    };

    class RendererSystem : public ISystem
    {
    public:
        RendererSystem(Camera2D* camera) : ISystem(camera) {};
        ~RendererSystem() { destroy(); }

        void init() override;
        void update(float dt) override;
        void destroy() override;

    private:
        Shader mTilemapShader;
        Shader mSpriteShader;
        std::vector<Layer> mLayers;
        std::vector<Renderable> mRenderables;
    };
} // namespace Plutus