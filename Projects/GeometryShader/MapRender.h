#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <Graphics/Shader.h>

namespace Plutus
{
    struct TileMapComponent;
    class Camera;

    struct Batch {
        uint32_t vertId;
        uint32_t buffId;
        int vertCount;
    };


    class MapRender
    {
    public:
        MapRender() = default;
        ~MapRender();
        void init(Camera* camera);
        void addMap(const std::string& id, TileMapComponent* tmap);
        void draw();

    private:
        int tileWidth;
        int tileHeight;
        std::vector<std::unordered_map<std::string, Batch>> tileMaps;

        Shader mShader;
        Camera* mCamera = nullptr;
    };
} // namespace Plutus