#pragma once

#include "RenderBatch.h"

#include <unordered_map>

#include "Vertex.h"
#include <Math/Vectors.h>

namespace Plutus
{
    struct TileMapComponent;

    struct TileVert : Vertex {
        int texIndex;

        TileVert() = default;
        TileVert(float x, float y, float ux, float uy, int ti) : texIndex(ti), Vertex(x, y, ux, uy) {}
        TileVert(const Vec2f& v, float ux, float uy, int ti) : texIndex(ti), Vertex(v.x, v.y, ux, uy) {}
    };

    class TileMapBatch : public RenderBatch
    {
    public:
        TileMapBatch(Camera2D* camera, Shader* shader) : RenderBatch(camera, shader) {  };
        ~TileMapBatch() { destroy(); }

        void init(TileMapComponent* tilemap);
        void draw(Shader* shader = nullptr) override;
        void update(float dt);
        void destroy() override;

        void loadTiles(TileMapComponent* tilemap);

    private:
        int texIndices[16];
        uint32_t mStaticTilesCount = 0;
        uint32_t animVert = 0;
        uint32_t staticVert = 0;

        uint32_t mIndexCount = 0;

        Texture* textures[16];
        std::vector<TileVert> mtiles;
        const uint32_t mVertexSize = sizeof(TileVert);
        TileMapComponent* mTileMap;
    };
} // namespace Plutus