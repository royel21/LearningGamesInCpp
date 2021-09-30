#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "Core/type.h"
#include "Graphics/Shader.h"
#include "Graphics/SpriteBatch2D.h"

namespace Plutus
{
    struct TileMap;

    class Scene;
    struct Tile;

    class TileMapPanel
    {
    private:
        Ref<Scene> mScene;
        Shader mShader;
        int mMode = 0;
        //TileMap
        std::vector<glm::ivec3> mTempTiles;
        TileMap* mTileMap = nullptr;
        Tile* mCurrentTile = nullptr;
        int mCurrentTexture = 0;
        float mRotation = 0;
        bool mShowAddModal = false;
        bool mIsOpen = false;
        SpriteBatch2D mRenderer;

    public:
        TileMapPanel() = default;
        ~TileMapPanel();
        void setContext(Ref<Scene>& scene);
        void draw(TileMap* tileMap);
        void drawTempTiles();
        void renderTiles(SpriteBatch2D* renderer, const glm::ivec2& mCoords);
        void createTiles(const glm::ivec2& mCoords);

    private:
        static bool compare(const glm::ivec2& a, const glm::ivec2& b);
        void tileProps();
        void addTexture(const char* label, bool& open, TileMap* tMap);
    };
} // namespace Plutus
