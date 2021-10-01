#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <Core/type.h>

namespace Plutus
{
    struct TileMap;
    class EditorUI;

    class Scene;
    struct Tile;

    class TileMapPanel
    {
    private:
        int mMode = 0;
        TileMap* mTileMap = nullptr;
        Tile* mCurrentTile = nullptr;
        int mCurrentTexture = 0;
        float mRotation = 0;
        bool mShowAddModal = false;
        bool mIsOpen = false;

        Ref<Scene> mScene;
        EditorUI* mParentUI;
        //Temp Tile List
        std::vector<glm::ivec3> mTempTiles;

    public:
        TileMapPanel() = default;
        void setContext(Ref<Scene>& scene, EditorUI* parent);
        void draw(TileMap* tileMap);
        void drawTempTiles();

    private:
        void tileProps();
        void renderTiles();
        void createTiles();
        void addTexture(const char* label, bool& open, TileMap* tMap);
    };
} // namespace Plutus
