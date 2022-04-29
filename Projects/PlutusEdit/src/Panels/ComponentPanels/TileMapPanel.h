#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Input/InputListener.h>

#include <Math/Vectors.h>

namespace Plutus
{
    struct Config;

    struct Tile;
    struct TileMapComponent;

    class TileMapPanel : public InputListener
    {
    public:
        TileMapPanel();
        void draw(Config* config);
        void onKeyDown(const std::string& key);
        void onMouseMove(float x, float y);

    private:
        void renderTemp();
        void createTiles();
        void processMode();
        void AddTexureDialog(bool& show);

    private:
        Config* mConfig;

        int mMode = 0;
        float mRotation = 0;
        bool mIsOpen = false;
        int mCurrentTexture = 0;
        bool mShowAddModal = false;
        TileMapComponent* mTileMap = nullptr;
        Tile* mCurrentTile = nullptr;
        std::vector<vec3i> mTempTiles;
    };
} // namespace Plutus