#pragma warning(disable: 4312)

#include "CenterPanel.h"
#include <imgui.h>
#include <imgui_internal.h>

#include <Input/Input.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>

#include "../Config.h"
#include "../Helpers/Render.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include "ECS/Components/Transform.h"

#include <Systems/Systems.h>

#include <Graphics/DebugRenderer.h>

#define mapIn(x, min_in, max_in, min_out, max_out) (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out

namespace Plutus
{
    CenterPanel::CenterPanel()
    {
        auto lang = TextEditor::LanguageDefinition::Lua();
        editor.SetPalette(TextEditor::GetDarkPalette());
        editor.SetLanguageDefinition(lang);
        editor.SetShowWhitespaces(false);

        scripts = Utils::listFiles("assets/script", ".lua");
        currentScript = scripts.size() ? scripts[0] : "";
        if (!currentScript.empty()) {
            editor.SetText(readFileAsString(currentScript.c_str()));
        }

        mSysManager.setScene(Config::get().mProject->mTempScene.get());
        mSysManager.AddSystem<ScriptSystem>(&Render::get().mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
        mSysManager.AddSystem<DebugSystem>(&Render::get().mCamera);
    }


    void CenterPanel::CameraControl()
    {
        if (Config::get().isHover) {
            auto pos = Config::get().mMouseCoords;
            auto& camera = Render::get().mCamera;

            if (Input::get()->onKeyPressed("F2"))
                camera.setPosition(0, 0);

            if (Input::get()->onKeyPressed("F3"))
                camera.setScale(1);

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
                    Config::get().vpPos = mCamCoords - result;
                    camera.setPosition(Config::get().vpPos);
                }

                auto scroll = Input::get()->getMouseWheel();
                if (scroll != 0)
                {
                    auto newVal = camera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
                    camera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
                }
            }
        }
    }

    void CenterPanel::SelectEntity()
    {
        auto project = Config::get().mProject;
        static Entity ent;
        auto& render = Render::get();
        if (Input::get()->onKeyPressed("MouseLeft"))
        {
            mMouseLastCoords = Config::get().mMouseCoords;
            auto camPos = render.mCamera.getPosition();
            mCamCoords = { camPos.x, camPos.y };
            ent = project->mScene->getEntity(render.mFramePicker.getEntId({ mMouseLastCoords.x, mMouseLastCoords.y }));

            if (ent) {
                project->mEnt = ent;
                if (ent.hasComponent<Plutus::Transform>()) {
                    auto pos = ent.getComponent<Transform>()->getPosition();
                    mEntLastPos = { pos.x, pos.y };
                }
            }
        }

        if (Input::get()->onKeyDown("MouseLeft") && Config::get().mProject->mScene->isValid(ent))
        {
            if (ent.hasComponent<Transform>()) {
                auto trans = ent.getComponent<Transform>();
                vec2f result = Config::get().mMouseCoords - mMouseLastCoords;
                result /= render.mCamera.getScale();

                trans->x = mEntLastPos.x + result.x;
                trans->y = mEntLastPos.y + result.y;
            }
        }
    }

    void CenterPanel::DrawViewPort() {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImVec4 WHITE = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, WHITE);
        auto& framebuffer = Render::get().mFrameBuffer;
        if (ImGui::BeginChild("v-port"))
        {
            auto winSize = framebuffer.getSize();
            float aspectRation = framebuffer.getAspectRatio();

            auto winPos = ImGui::GetContentRegionAvail();

            vec2f newSize(winPos.x, winPos.x / aspectRation);
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
            if (Config::get().isHover = ImGui::IsItemHovered())
            {
                Config::get().mMouseCoords = { xPos, yPos };

                CameraControl();
            }

            auto curPos = ImGui::GetWindowSize();
            ImGui::SetCursorPos({ 0, curPos.y - 20.0f });
            ImGui::PushStyleColor(ImGuiCol_Text, { 1,0,0,1 });
            ImGui::Text("FPS: %0.2f", ImGui::GetIO().Framerate);
            ImGui::PopStyleColor();

            ImGui::EndChild();
        }
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(1);
        SelectEntity();

    }
    void CenterPanel::update(float dt) {
        if (Config::get().state == Running)
            mSysManager.update(dt);
    }
    void CenterPanel::DrawCenterPanel()
    {

        auto& project = Config::get().mProject;

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin("Viewport Window");
        bool isViewPort = true;
        ImGui::BeginTabBar("center-tabbar");
        {
            if (ImGui::BeginTabItem("ViewPort")) {
                DrawViewPort();
                ImGui::EndTabItem();
            }
            if (Config::get().state == Editing && ImGui::BeginTabItem("Script Editor")) {
                editor.Render("TextEditor");
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
            bool isRunning = Config::get().state == Running;

            if (ImGui::TransparentButton(isRunning ? ICON_FA_STOP : ICON_FA_PLAY)) {
                if (isRunning) {
                    Render::get().setScene(project->mScene.get());
                    Config::get().state = Editing;
                    project->mTempScene->clear();
                    mSysManager.stop();

                    DebugRender::get()->setShouldDraw(drawGrid);
                }
                else {
                    project->mTempScene.get()->copyScene(project->mScene.get());
                    Render::get().setScene(project->mTempScene.get());
                    Config::get().state = Running;
                    mSysManager.start();

                    drawGrid = DebugRender::get()->getShouldDraw();
                    DebugRender::get()->setShouldDraw(false);
                }
            }
            ImGui::SameLine();
            ImGui::TransparentButton(ICON_FA_COG);
        }
        else {
            ImGui::TransparentButton(ICON_FA_FILE " New");
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_SAVE " Save")) {
                Utils::saveFile(currentScript.c_str(), editor.GetText().c_str());
            }
            ImGui::SameLine();
            ImGui::PushItemWidth(200);
            int selected = Utils::getIndex(scripts, currentScript);
            if (ImGui::ComboBox("##scr-list", scripts, selected)) {
                currentScript = scripts[selected];
                editor.SetText(readFileAsString(currentScript.c_str()));
            }
            ImGui::PopItemWidth();
        }

        ImGui::End();
    }
}