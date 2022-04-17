#pragma warning(disable: 4312)

#include "CenterWindow.h"
#include <imgui.h>
#include <imgui_internal.h>

#include <Input/Input.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>

#include "../Config.h"
#include "../Helpers/Render.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include "ECS/Components/TransformComponent.h"

#include <Systems/Systems.h>

#include <Graphics/DebugRenderer.h>

#include <Log/Logger.h>

#define mapIn(x, min_in, max_in, min_out, max_out) (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out

namespace Plutus
{
    void CenterWindow::init(Config* config)
    {
        mConfig = config;

        auto lang = TextEditor::LanguageDefinition::Lua();
        mTextEditor.SetPalette(TextEditor::GetDarkPalette());
        mTextEditor.SetLanguageDefinition(lang);
        mTextEditor.SetShowWhitespaces(false);

        scripts = Utils::listFiles("assets/script", ".lua");
        currentScript = scripts.size() ? scripts[0] : "";
        if (!currentScript.empty()) {
            mTextEditor.SetText(readFileAsString(currentScript.c_str()));
        }

        mSysManager.AddSystem<ScriptSystem>(&mConfig->mRender->mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
    }


    void CenterWindow::CameraControl()
    {
        if (mConfig->isHover) {
            auto pos = mConfig->mMouseCoords;
            auto& camera = mConfig->mRender->mCamera;

            if (Input::get()->onKeyPressed("F2"))
                camera.setPosition(0, 0);

            if (Input::get()->onKeyPressed("F3")) {
                camera.setScale(1);
                mConfig->vpZoom = 1;
            }

            if (Input::get()->onKeyPressed("MouseLeft"))
            {
                mMouseLastCoords = pos;
                mCamCoords = camera.getPosition();
            }
            // move the camera
            if (Input::get()->isCtrl)
            {
                if (Input::get()->onKeyDown("MouseLeft"))
                {
                    vec2f result = pos - mMouseLastCoords;
                    result /= camera.getScale();
                    mConfig->vpPos = mCamCoords - result;
                }

                auto scroll = Input::get()->getMouseWheel();
                if (scroll != 0)
                {
                    auto scalePos = pos / camera.getScale();

                    auto newVal = camera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
                    camera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
                    mConfig->vpZoom = camera.getScale();

                    auto newPos = pos / camera.getScale();

                    auto offset = newPos - scalePos;
                    mConfig->vpPos = camera.getPosition() - offset;
                }

                // mConfig->vpPos = { roundf(mConfig->vpPos.x), floor(mConfig->vpPos.y) };

                camera.setPosition(mConfig->vpPos);
            }
        }
    }

    void CenterWindow::selectEntity()
    {
        auto project = mConfig->mProject;
        static Entity ent;
        if (!Input::get()->isCtrl) {
            if (Input::get()->onKeyPressed("MouseLeft"))
            {
                mMouseLastCoords = mConfig->mMouseCoords;
                auto camPos = mConfig->mRender->mCamera.getPosition();
                mCamCoords = { camPos.x, camPos.y };
                ent = project->mScene->getEntity(mConfig->mRender->mFramePicker.getEntId(mMouseLastCoords));

                if (ent) {
                    project->mEnt = ent;
                    if (ent.hasComponent<TransformComponent>()) {
                        auto pos = ent.getComponent<TransformComponent>()->getPosition();
                        mEntLastPos = { pos.x, pos.y };
                    }
                }
            }

            if (Input::get()->onKeyDown("MouseLeft") && mConfig->mProject->mScene->isValid(ent))
            {
                if (ent.hasComponent<TransformComponent>()) {
                    auto trans = ent.getComponent<TransformComponent>();
                    vec2f result = mConfig->mMouseCoords - mMouseLastCoords;
                    result /= mConfig->mRender->mCamera.getScale();

                    trans->x = mEntLastPos.x + result.x;
                    trans->y = mEntLastPos.y + result.y;
                }
            }
        }
    }

    void CenterWindow::drawViewPort() {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImVec4 WHITE = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, WHITE);
        auto& framebuffer = mConfig->mRender->mFrameBuffer;
        if (ImGui::BeginChild("v-port"))
        {
            auto winSize = framebuffer.getSize();
            float aspectRation = framebuffer.getAspectRatio();

            auto winPos = ImGui::GetContentRegionAvail();

            vec2f newSize((int)winPos.x, int(winPos.x / aspectRation));
            if (newSize.y > winPos.y)
            {
                newSize.y = winPos.y;
                newSize.x = winPos.y * aspectRation;
            }
            // add padding to the canvas
            newSize -= 10;
            //calculate the position centered oncanvas
            float x = std::max((winPos.x - newSize.x), 0.0f) * 0.5f;
            float y = std::max((winPos.y - newSize.y), 0.0f) * 0.5f;
            //set the new posotion
            ImGui::SetCursorPos({ x, y });

            ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

            float xPos = mapIn(ImGui::GetIO().MousePos.x - canvas_pos.x, 0, newSize.x, 0, winSize.x);
            float yPos = winSize.y - mapIn(ImGui::GetIO().MousePos.y - canvas_pos.y, 0, newSize.y, 0, winSize.y);

            ImGui::Image((void*)framebuffer.getTextureId(), { newSize.x, newSize.y }, { 0, 1 }, { 1, 0 }, WHITE, { 0.0, 0.0, 0.0, 1.0 });
            if (mConfig->isHover = ImGui::IsItemHovered())
            {
                mConfig->mMouseCoords = { xPos, yPos };

                CameraControl();

                if (mConfig->state != Running)
                    selectEntity();
            }

            auto curPos = ImGui::GetWindowSize();
            ImGui::SetCursorPos({ 0, curPos.y - 20.0f });
            ImGui::PushStyleColor(ImGuiCol_Text, { 1,0,0,1 });
            ImGui::Text("FPS: %0.2f", ImGui::GetIO().Framerate);
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(1);
    }

    void CenterWindow::update(float dt) {
        if (mConfig->state == Running)
            mSysManager.update(dt);
    }

    void CenterWindow::draw()
    {

        auto& project = mConfig->mProject;

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
        if (ImGui::Begin("Viewport Window")) {
            bool isViewPort = true;
            ImGui::BeginTabBar("center-tabbar");
            {
                if (ImGui::BeginTabItem("ViewPort")) {
                    drawViewPort();
                    ImGui::EndTabItem();
                }

                if (mConfig->state == Editing && ImGui::BeginTabItem("Script Editor")) {
                    mTextEditor.Render("TextEditor");
                    isViewPort = false;
                    ImGui::EndTabItem();
                }

            }
            ImGui::EndTabBar();

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f);
            ImGui::SetCursorPosY(4);
            if (isViewPort) {
                static bool drawGrid = DebugRender::get()->getShouldDraw();
                bool isRunning = mConfig->state == Running;

                if (ImGui::TransparentButton(isRunning ? ICON_FA_STOP : ICON_FA_PLAY)) {
                    if (isRunning) {
                        mConfig->mRender->setScene(project->mScene.get());
                        mConfig->state = Editing;
                        project->mTempScene->clear();
                        mSysManager.stop();
                        DebugRender::get()->setShouldDraw(mConfig->drawGrid);
                    }
                    else {
                        mConfig->state = Running;

                        project->mTempScene.get()->copyScene(project->mScene.get());
                        DebugRender::get()->setShouldDraw(false);

                        mSysManager.setScene(mConfig->mProject->mTempScene.get());
                        mSysManager.init();
                        mConfig->mRender->setScene(mConfig->mProject->mTempScene.get());
                    }
                }
                ImGui::SameLine();
                ImGui::TransparentButton(ICON_FA_COG);
            }
            else {
                ImGui::TransparentButton(ICON_FA_FILE " New");
                ImGui::SameLine();
                if (ImGui::TransparentButton(ICON_FA_SAVE " Save")) {
                    Utils::saveFile(currentScript.c_str(), mTextEditor.GetText().c_str());
                }
                ImGui::SameLine();
                ImGui::PushItemWidth(200);
                int selected = Utils::getIndex(scripts, currentScript);
                if (ImGui::ComboBox("##scr-list", scripts, selected)) {
                    currentScript = scripts[selected];
                    mTextEditor.SetText(readFileAsString(currentScript.c_str()));
                }
                ImGui::PopItemWidth();
            }

        }
        ImGui::End();
    }
}