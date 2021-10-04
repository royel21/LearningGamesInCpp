
#include "EntityEditor.h"
#include "ImGuiEx.h"
#include "Input/Input.h"
#include "Assets/AssetManager.h"
#include "ECS/Scene.h"
#include "IconsFontAwesome5.h"
#include "Utils/Utils.h"
#include "EditorUI.h"
#include "Modals.h"

namespace Plutus
{
    void EntityEditor::setContext(const Ref<Scene>& context, EditorUI* parent)
    {
        mScene = context;
        mParentUI = parent;
        mComponentPanel.setContext(mScene, parent);
        mInput = Plutus::Input::getInstance();
    }

    void EntityEditor::draw()
    {
        bool open = true;
        ImGui::Begin("Scene Editor", &open, ImGuiWindowFlags_NoScrollbar);
        entityList();
        ImGui::End();
    }

    void EntityEditor::entityList()
    {
        static uint32_t selected = 0;
        mScene->getRegistry()->each([&](auto entId)
            {
                Entity entity{ entId , mScene.get() };
                drawEntity(entity);
            });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            mParentUI->setEntity({});

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity")) {
                mParentUI->setEntity(mScene->createEntity("Empty Entity"));
            }
            ImGui::EndPopup();
        }

        mComponentPanel.drawUI();
    }

    void EntityEditor::drawEntity(Entity ent)
    {
        bool isCurrent = mParentUI->getEntity() == ent;
        auto& tag = ent.getName();

        ImGuiTreeNodeFlags flags = (isCurrent ? ImGuiTreeNodeFlags_Selected : 0);
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            mParentUI->setEntity(ent);
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem(("Delete " + tag).c_str()))
            {
                if (isCurrent)
                    mParentUI->setEntity({});
                mScene->removeEntity(ent);
            }

            ImGui::EndPopup();
        }
    }
} // namespace Plutus