#pragma once
#include <vector>
#include "imgui.h"
#include "TileMapPanel.h"
#include "Core/type.h"
#include <functional>
#include <unordered_map>

namespace Plutus
{
    class Entity;
    struct TileSet;
    class Input;
    class Scene;
    class EditorUI;
    class SpriteBatch2D;

    class ComponentPanel
    {
    private:
        Entity* mEntity = nullptr;
        Input* mInput = nullptr;
        std::vector<ImVec2> mSelectedtiles;
        Ref<Scene> mScene;
        TileMapPanel mTileMapPanel;
        glm::vec2 mGridCoord;
        std::unordered_map < std::string, std::function<void(bool&)>> createComps;

    public:
        ComponentPanel() = default;

        void setContext(Ref<Scene>& scene);
        void drawUI(Entity* ent);
        void render(SpriteBatch2D* renderer, glm::vec2 mcoords);
        inline void createTiles(const glm::vec2& mCoords) { mTileMapPanel.createTiles(mCoords); }

    private:
        void drawAnimate();
        void drawTransform();
        void drawSprite();
        void drawTileMap();
        void drawScript();

        void drawCanvas(TileSet* tileset, float scale);
        void drawTexCoords(TileSet* tileset, float scale);
        void drawTilesetProps(TileSet* tileset);
        bool showCreateComp(bool& open);
        std::vector<std::string> getCompList();

        void showDialog(const char* name, float width, float height);
        void endDialog(bool& show);
    };
} // namespace Plutus