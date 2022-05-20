#pragma once

#include "RenderBatch.h"

namespace Plutus
{
    struct TileMapComponent;

    class TileMapBatch : public RenderBatch
    {
    public:
        TileMapBatch(Camera2D* camera, Shader* shader) : RenderBatch(camera, shader) {  };
        ~TileMapBatch() { destroy(); }

        void init(TileMapComponent* tilemap);
        void draw() override;
        void destroy() override;
    };
} // namespace Plutus