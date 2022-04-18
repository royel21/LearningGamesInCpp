
#include "SceneWindow.h"

#include "../Helpers/ImGuiEx.h"
#include "../Helpers/ImGuiDialog.h"
#include "../Helpers/IconsFontAwesome5.h"

#include <Log/Logger.h>

namespace Plutus
{
    void SceneWindow::draw() {
        auto& project = mConfig->mProject;
        static bool openNew = false;
        static bool showConfig = false;
        if (ImGui::Begin("Scene Editor"))
        {
            // ImGui::Push
            ImGui::Text("Scenes");
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE "##new-scene"))  openNew = true;
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_SAVE "##save-scene"))  project->Save();
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_COG "##config-scene")) showConfig = true;
            ImGui::Separator();
            for (auto& sc : project->mScenes) {

                bool isOpen = project->mOpenScene.compare(sc.first) == 0;
                ImGui::SetNextItemOpen(isOpen);
                bool isNodeOpen = ImGui::TreeNode((ICON_FA_PHOTO_VIDEO " " + sc.first).c_str());

                if (isNodeOpen)
                {
                    //Load Scene only once
                    if (!isOpen) {
                        project->Load(sc.second.c_str());
                    }

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
                                drawEntity({ entId , project->mScene.get() });
                            });
                    }
                    project->mOpenScene = sc.first;
                    ImGui::TreePop();
                }
                else  if (isOpen) {
                    project->mOpenScene = "";
                }
            }
        }
        ImGui::End();

        if (openNew) ImGui::NewFileDialig("New Project", [&](const std::string& name) {
            if (!name.empty())  project->Create(name);
            openNew = false;
            });

        if (showConfig) showConfigScene(showConfig, project->mScene);
    }

    void SceneWindow::drawEntity(Entity ent)
    {
        auto project = mConfig->mProject;
        bool isCurrent = project->mEnt == ent;
        auto& tag = ent.getName();

        ImGuiTreeNodeFlags flags = (isCurrent ? ImGuiTreeNodeFlags_Selected : 0);
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

        ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, (ICON_FA_MOBILE " " + tag).c_str());
        if (ImGui::IsItemClicked())
        {
            project->mEnt = ent;
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem(("Copy " + tag).c_str())) {
                project->mEnt = project->mScene->CreateCopy(ent);
            }

            if (ImGui::MenuItem(("Delete " + tag).c_str()))
            {
                if (isCurrent)
                    project->mEnt = ent;
                project->mScene->removeEntity(ent);
            }
            ImGui::EndPopup();
        }
    }

    void SceneWindow::showConfigScene(bool& show, Ref<Scene>& scene) {
        ImGui::SetNextWindowSize({ 350, 0 });
        ImGui::BeginDialog("Config Scene");
        {
            ImGui::Text("Box2d Config");
            ImGui::Separator();
            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {
                ImGui::BeginCol("Gravity");
                vec2f g = scene->getGravity();
                if (ImGui::Draw2Float("##gravity", g)) {
                    scene->setGravity(g);
                }

                ImGui::BeginCol("Time Step FPS");
                float timeIter = 1 / scene->getTimeIterSec();
                if (ImGui::DragFloat("##timeIter", &timeIter, 1, 10, 240, "%0.0f")) {
                    scene->setTimeIterSec(timeIter);
                }

                ImGui::BeginCol("Velocity Iter");
                int vIter = scene->getVelIter();
                if (ImGui::DragInt("##viter", &vIter, 1, 1, 20)) {
                    scene->setVelIter(vIter);
                }
                ImGui::BeginCol("Body Iter");
                int bIter = scene->getPositionIter();
                if (ImGui::DragInt("##biter", &bIter, 1, 1, 20)) {
                    scene->setPositionIter(bIter);
                }

                ImGui::BeginCol("Auto Clear Force");
                bool clearForce = scene->getAutoClearForce();
                if (ImGui::Checkbox("##cforce", &clearForce)) {
                    scene->setAutoClearForce(clearForce);
                }

                ImGui::EndUIGroup();
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Close##modal-1"))
            show = false;
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }

} // namespace Plutus