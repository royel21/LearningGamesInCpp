#pragma once

#include <vector>
#include <imgui.h>
#include <Core/type.h>
#include <functional>
#include <unordered_map>

#include "AnimationPanel.h"
#include "TileMapPanel.h"
#include <ECS/Scene.h>

namespace Plutus
{
    struct Texture;
    class Input;
    class EditorUI;

    class ComponentPanel
    {
    private:
        Entity mEntity;
        Input* mInput = nullptr;
        Ref<Scene> mScene;
        EditorUI* mParentUI;
        glm::vec2 mGridCoord;
        TileMapPanel mTileMapPanel;
        AnimationPanel mAnimPanel;

        std::vector<ImVec2> mSelectedtiles;
        std::unordered_map<std::string, std::function<void(bool&)>> createComps;

    public:
        ComponentPanel() = default;

        void setContext(Ref<Scene>& scene, EditorUI* parentUI);

        void drawUI();

    private:
        template<typename T>
        bool CollapseComponent(char* label, int id);
        void drawTransform();
        void drawSprite();
        void drawAnimation();
        void drawScript();
        void drawMapTile();

        void drawCanvas(Texture* tileset, float scale);
        void drawTexCoords(Texture* tileset, float scale);
        void drawTilesetProps(Texture* tileset);
        bool showCreateComp(bool& open);
        std::vector<std::string> getCompList();

        void showDialog(const char* name, float width, float height);
        void endDialog(bool& show);
    };
} // namespace Plutus