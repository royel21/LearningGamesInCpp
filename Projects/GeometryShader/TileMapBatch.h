#pragma once

#include "RenderBatch.h"

#include <unordered_map>

#include "Vertex.h"

namespace Plutus
{
    struct TileMapComponent;

    struct TileVert : Vertex {
        int texIndex;
        TileVert(float x, float y, float ux, float uy, int ti) : texIndex(ti), Vertex(x, y, ux, uy) {}
    };

    class TileMapBatch : public RenderBatch
    {
    public:
        TileMapBatch(Camera2D* camera, Shader* shader) : RenderBatch(camera, shader) {  };
        ~TileMapBatch() { destroy(); }

        void init(TileMapComponent* tilemap);
        void draw() override;
        void destroy() override;

        void loadTiles(TileMapComponent* tilemap);

    private:
        int texIndices[16];
        Texture* textures[16];
        uint32_t mVerCount = 0;
        std::vector<TileVert> tiles;
    };
} // namespace Plutus