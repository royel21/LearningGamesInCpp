
#include "ScenesList.h"

#include "../Context.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include "../Helpers/Config.h"

#include <Serialize/SceneLoader.h>
#include <Assets/AssetManager.h>

namespace Plutus
{
    void drawEntity(Entity ent)
    {
        bool isCurrent = Context.mEnt == ent;
        auto& tag = ent.getName();

        ImGuiTreeNodeFlags flags = (isCurrent ? ImGuiTreeNodeFlags_Selected : 0);
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, (ICON_FA_MOBILE " " + tag).c_str());
        if (ImGui::IsItemClicked())
        {
            Context.mEnt = ent;
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem(("Delete " + tag).c_str()))
            {
                if (isCurrent)
                    Context.mEnt = ent;
                Context.mScene->removeEntity(ent);
            }
            if (ImGui::MenuItem(("Copy " + tag).c_str())) {

            }
            ImGui::EndPopup();
        }
    }

    void DrawScenes() {
        ImGui::Begin("Scene Editor");

        static uint32_t selected = 0;
        int id = 0xF480;
        for (auto& sc : Config.project) {
            bool isOpen = Context.currentScene.compare(sc.first) == 0;
            ImGui::SetNextItemOpen(isOpen);
            if (ImGui::TreeNode((ICON_FA_PHOTO_VIDEO " " + sc.first).c_str())) {

                if (!isOpen) {
                    Context.mEnt = {};
                    Context.mScene->clear();
                    AssetManager::get()->clearData();
                    SceneLoader::loadFromJson(sc.second.c_str(), Context.mScene);
                }
                Context.mScene->getRegistry()->each([&](auto entId)
                    {
                        Entity entity{ entId , Context.mScene.get() };
                        drawEntity(entity);
                    });
                Context.currentScene = sc.first;
                ImGui::TreePop();
            }
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            Context.mEnt = {};

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Entity")) {
                Context.mEnt = Context.mScene->createEntity("New Entity");
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }
} // namespace Plutus