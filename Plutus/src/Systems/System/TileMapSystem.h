#pragma once
#include "System.h"
#include <vector>
#include <unordered_map>

#include <stdint.h>

#include <Graphics/Shader.h>

namespace Plutus
{
    struct TileMapComponent;
    struct Batch {
        uint32_t vertId;
        uint32_t buffId;
        int vertCount;
    };

    class TileMapSystem : public ISystem
    {
    public:
        TileMapSystem(Camera2D* camera) : ISystem(camera) {  };
        ~TileMapSystem() { destroy(); }

        void init();
        void update(float dt);
        void destroy() override;

    private:
        int tileWidth;
        int tileHeight;
        std::vector<std::unordered_map<std::string, Batch>> tileMaps;

        Shader mShader;

        void addMap(const std::string& id, TileMapComponent* tmap);
    };
} // namespace Plutus