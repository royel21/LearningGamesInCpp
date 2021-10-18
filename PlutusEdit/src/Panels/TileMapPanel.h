#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Input/InputListener.h>

namespace Plutus
{

    struct Tile;
    struct TileMap;

    class TileMapPanel : public InputListener
    {
    public:
        TileMapPanel();
        void DrawTileMapComponet();
        void onKeyDown(const std::string& key);
        void onMouseMove(float x, float y);

    private:
        void selectTile();

    private:
        int mMode = 0;
        TileMap* mTileMap = nullptr;
        Tile* mCurrentTile = nullptr;
        int mCurrentTexture = 0;
        float mRotation = 0;
        bool mShowAddModal = false;
        bool mIsOpen = false;
        std::vector<glm::ivec3> mTempTiles;
    };
} // namespace Plutus