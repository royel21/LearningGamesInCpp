
#include "SceneWindow.h"

#include "../Helpers/ImGuiEx.h"
#include "../Helpers/ImGuiDialog.h"
#include "../Helpers/IconsFontAwesome5.h"

#include "../Config.h"

#include <misc/cpp/imgui_stdlib.h>

#include <Assets/Assets.h>
#include <ECS/Components/TagComponent.h>

namespace Plutus
{
    void SceneWindow::draw() {
        if (mConfig->state != Editing) return;

        auto& project = mConfig->mProject;
        static bool openNew = false;
        static bool showConfig = false;
        std::string toRemove = "";
        if (ImGui::Begin("Scene Editor"))
        {
            float width = ImGui::GetContentRegionAvailWidth();
            // ImGui::Push
            ImGui::Text("Scenes");
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE "##new-scene"))  openNew = true;
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_SAVE "##save-scene"))  project.save(mConfig->mProjects[mConfig->currentProject]);

            bool isNodeOpen = false;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            for (auto& sc : project.scenes) {

                bool isOpen = project.currentScene.compare(sc.first) == 0;

                ImGui::SetNextItemOpen(isOpen);
                bool showContent = ImGui::TreeNodeEx(sc.first.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap, (ICON_FA_PHOTO_VIDEO " " + sc.first).c_str());
                ImGui::SameLine(width - 50);
                if (ImGui::ButtonIcon((ICON_FA_EDIT " ##ren-" + sc.first).c_str(), { 0,0,1,1 })) {
                    newName = sceneName = sc.first;
                    ImGui::OpenPopup("Rename Scene");
                }
                ImGui::SameLine();
                if (ImGui::ButtonIcon((ICON_FA_TRASH " ##rm-" + sc.first).c_str(), { 1,0,0,1 })) {
                    if (isOpen) {
                        showContent = false;
                        ImGui::TreePop();
                    }
                    toRemove = sc.first;
                }

                if (showContent)
                {
                    isNodeOpen = true;
                    //Load Scene only once
                    if (!isOpen) {
                        project.loadScene(sc.first);
                    }

                    if (isOpen) {
                        project.scene->getRegistry()->each([&](auto entId)
                            {
                                drawEntity({ entId , project.scene.get() });
                            });
                    }
                    project.currentScene = sc.first;
                    ImGui::TreePop();
                }
                else  if (isOpen) {
                    project.unLoadScene();
                }
            }
            ImGui::PopStyleVar();

            if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
                ImGui::OpenPopup("AssetMenu");
            }

            if (ImGui::BeginPopup("AssetMenu"))
            {
                if (!isNodeOpen) {
                    if (ImGui::MenuItem("Create Scene")) {
                        auto count = std::to_string(project.scenes.size());
                        project.CreateScene("New Scene  - " + count);
                    }
                }
                else {
                    if (ImGui::MenuItem("Create Entity")) {
                        auto count = std::to_string(project.scene->getRegistry()->size());
                        project.mEnt = project.scene->createEntity("New Entity - " + count);
                    }
                }
                ImGui::EndPopup();
            }

            if (ImGui::BeginModal("Rename Scene", { 300, 0 })) {
                ImGui::Row("Name");
                ImGui::InputText("##label-rn-sc", &newName);
                ImGui::EndModal([&](bool save) {
                    if (save) {
                        if (newName.compare(sceneName) != 0) {
                            project.RenameScene(sceneName, newName);
                        }
                    }
                    });
            }
        }
        ImGui::End();

        if (!toRemove.empty()) {
            project.scenes.erase(toRemove);
        }
    }

    void SceneWindow::drawEntity(Entity ent)
    {
        auto& project = mConfig->mProject;
        bool isCurrent = project.mEnt == ent;
        auto tag = ent.getComponent<TagComponent>();
        float width = ImGui::GetContentRegionAvailWidth();

        ImGuiTreeNodeFlags flags = (isCurrent ? ImGuiTreeNodeFlags_Selected : 0);
        flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen
            | ImGuiTreeNodeFlags_Leaf
            | ImGuiTreeNodeFlags_AllowItemOverlap;

        ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, (ICON_FA_MOBILE " " + tag->Name).c_str());
        float pos = ImGui::GetMousePos().x - ImGui::GetItemRectSize().x;

        if (ImGui::IsItemClicked())
        {
            project.mEnt = ent;
        }

        ImGui::SameLine(width - 10);
        if (ImGui::TransparentButton(((tag->Visible ? ICON_FA_EYE : ICON_FA_EYE_SLASH) + std::string(" ##cp-v") + tag->Name).c_str())) {
            tag->Visible = !tag->Visible;
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem(("Copy " + tag->Name).c_str())) {
                project.mEnt = project.scene->CreateCopy(ent);
            }

            if (ImGui::MenuItem(("Delete " + tag->Name).c_str()))
            {
                if (isCurrent)
                    project.mEnt = ent;
                project.scene->removeEntity(ent);
            }
            ImGui::EndPopup();
        }
    }

    void SceneWindow::showConfigScene(bool& show, Ref<Scene>& scene) {
        // ImGui::SetNextWindowSize({ 350, 0 });
        // ImGui::BeginDialog("Config Scene");
        // {
        //     ImGui::Text("Box2d Config");
        //     ImGui::Separator();
        //     if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {
        //         ImGui::BeginCol("Gravity");
        //         Vec2f g = scene->getGravity();
        //         if (ImGui::Draw2Float("##gravity", g)) {
        //             scene->setGravity(g);
        //         }

        //         ImGui::BeginCol("Time Step FPS");
        //         float timeIter = 1 / scene->getTimeIterSec();
        //         if (ImGui::DragFloat("##timeIter", &timeIter, 1, 10, 240, "%0.0f")) {
        //             scene->setTimeIterSec(timeIter);
        //         }

        //         ImGui::BeginCol("Velocity Iter");
        //         int vIter = scene->getVelIter();
        //         if (ImGui::DragInt("##viter", &vIter, 1, 1, 20)) {
        //             scene->setVelIter(vIter);
        //         }
        //         ImGui::BeginCol("Body Iter");
        //         int bIter = scene->getPositionIter();
        //         if (ImGui::DragInt("##biter", &bIter, 1, 1, 20)) {
        //             scene->setPositionIter(bIter);
        //         }

        //         ImGui::BeginCol("Auto Clear Force");
        //         bool clearForce = scene->getAutoClearForce();
        //         if (ImGui::Checkbox("##cforce", &clearForce)) {
        //             scene->setAutoClearForce(clearForce);
        //         }

        //         ImGui::EndUIGroup();
        //     }
        // }
        // ImGui::Separator();
        // if (ImGui::Button("Close##modal-1"))
        //     show = false;
        // ImGui::PopStyleColor();
        // ImGui::EndPopup();
    }

} // namespace Plutus