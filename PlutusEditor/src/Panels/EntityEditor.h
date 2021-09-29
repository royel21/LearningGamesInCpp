#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "imgui.h"
#include "glm/glm.hpp"
#include "Core/type.h"
#include <Panels/ComponentPanel.h>

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
    ComponentPanel mComponentPanel;

    class EntityEditor
    {
    public:
        EntityEditor() = default;

        void draw();
        void setContext(const Ref<Scene>& context, EditorUI* parent);

    private:
        void entityList();
        void drawEntity(Entity ent);

    private:
        Input* mInput = nullptr;

        Ref<Scene> mScene;
        EditorUI* mParentUI;
    };
} // namespace Plutus