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
        void renderTemp();
        void createTiles();
        void processMode();
        void AddTexureDialog(bool& show);

    private:
        int mMode = 0;
        float mRotation = 0;
        bool mIsOpen = false;
        int mCurrentTexture = 0;
        bool mShowAddModal = false;
        TileMap* mTileMap = nullptr;
        Tile* mCurrentTile = nullptr;
        std::vector<glm::ivec3> mTempTiles;
    };
} // namespace Plutus