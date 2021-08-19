#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "imgui.h"
#include "glm/glm.hpp"
#include "Core/type.h"

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

namespace Plutus
{
    class Input;
    class Scene;
    class Entity;
    class EditorUI;
    struct Layer;

    class EntityEditor
    {
    public:
        EntityEditor() = default;
        void setContext(const Ref<Scene>& context, EditorUI* parent);
        void draw();
        void loadScene(std::string path);

    private:
        void layers();
        void entity();
        void drawTileset();
        std::string LayerModal(char* label, bool* open);

    private:
        int mMode;
        glm::vec2 modalPos;
        std::vector<ImVec2> mSelectedtiles;
        bool mShowCreateLayer = false;
        Input* mInput = nullptr;

        Ref<Scene> mScene;
        std::unordered_map<std::string, Layer>* mLayers;
        EditorUI* mParentUI;
    };
} // namespace Plutus