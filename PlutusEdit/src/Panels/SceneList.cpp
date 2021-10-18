
#include "ScenesList.h"

#include "../Config.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/ImGuiDialog.h"
#include "../Helpers/IconsFontAwesome5.h"

namespace Plutus
{
    void drawEntity(Entity ent)
    {
        auto project = Config::get().mProject;
        bool isCurrent = project->mEnt == ent;
        auto& tag = ent.getName();

        ImGuiTreeNodeFlags flags = (isCurrent ? ImGuiTreeNodeFlags_Selected : 0);
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, (ICON_FA_MOBILE " " + tag).c_str());
        if (ImGui::IsItemClicked())
        {
            project->mEnt = ent;
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem(("Delete " + tag).c_str()))
            {
                if (isCurrent)
                    project->mEnt = ent;
                project->mScene->removeEntity(ent);
            }
            if (ImGui::MenuItem(("Copy " + tag).c_str())) {

            }
            ImGui::EndPopup();
        }
    }

    void DrawScenes() {
        auto project = Config::get().mProject;
        static bool openNew = false;
        ImGui::Begin("Scene Editor");
        if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE "##new-scene"))  openNew = true;
        ImGui::SameLine();
        if (ImGui::TransparentButton(ICON_FA_SAVE "##save-scene"))  project->Save();
        ImGui::SameLine();
        if (ImGui::TransparentButton(ICON_FA_COG "##config-scene")) {}
        ImGui::SameLine();
        ImGui::Text("Scenes");
        ImGui::Separator();
        for (auto& sc : project->mScenes) {
            bool isOpen = project->mOpenScene.compare(sc.first) == 0;

            ImGui::SetNextItemOpen(isOpen);

            if (ImGui::TreeNode((ICON_FA_PHOTO_VIDEO " " + sc.first).c_str()))
            {
                //Load Scene only once
                if (!isOpen) project->Load(sc.second.c_str());

                // Right-click on scene
                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Create Entity")) {
                        project->mEnt = project->mScene->createEntity("New Entity");
                    }
                    ImGui::EndPopup();
                }

                if (isOpen) {
                    project->mScene->getRegistry()->each([&](auto entId)
                        {
                            Entity entity{ entId , project->mScene.get() };
                            drawEntity(entity);
                        });
                }
                project->mOpenScene = sc.first;
                ImGui::TreePop();
            }
        }
        ImGui::End();

        if (openNew) ImGui::NewFileDialig("New Project", [&](const std::string& name) {
            if (!name.empty())  project->Create(name);
            openNew = false;
            });
    }
} // namespace Plutus