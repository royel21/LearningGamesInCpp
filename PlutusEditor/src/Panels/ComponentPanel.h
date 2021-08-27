#pragma once
#include <vector>
#include "imgui.h"
#include "Core/type.h"
#include <functional>
#include <unordered_map>

#include "AnimationPanel.h"
#include "TileMapPanel.h"

namespace Plutus
{
    class Entity;
    struct Texure;
    class Input;
    class Scene;
    class EditorUI;
    class SpriteBatch2D;

    class ComponentPanel
    {
    private:
        Entity *mEntity = nullptr;
        Input *mInput = nullptr;
        std::vector<ImVec2> mSelectedtiles;
        Ref<Scene> mScene;
        glm::vec2 mGridCoord;
        std::unordered_map<std::string, std::function<void(bool &)>> createComps;
        TileMapPanel mTileMapPanel;
        AnimationPanel mAnimPanel;

    public:
        ComponentPanel() = default;

        void setContext(Ref<Scene> &scene);
        void drawUI(Entity *ent);
        void render(SpriteBatch2D *renderer, glm::vec2 mcoords);
        inline void createTiles(const glm::vec2 &mCoords) { mTileMapPanel.createTiles(mCoords); }

    private:
        void drawTransform();
        void drawSprite();
        void drawScript();

        void drawCanvas(Texure *tileset, float scale);
        void drawTexCoords(Texure *tileset, float scale);
        void drawTilesetProps(Texure *tileset);
        bool showCreateComp(bool &open);
        std::vector<std::string> getCompList();

        void showDialog(const char *name, float width, float height);
        void endDialog(bool &show);
    };
} // namespace Plutus