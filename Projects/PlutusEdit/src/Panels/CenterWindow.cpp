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


#include <Systems/Systems.h>

#include <Graphics/DebugRenderer.h>
#include <Assets/Assets.h>
#include <ECS/Components/TransformComponent.h>

#include <Time/Timer.h>

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

        mSysManager.setProject(&mConfig->mTempProject);
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
                mConfig->mProject.zoomLevel = 1;
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
                    Vec2f result = pos - mMouseLastCoords;
                    result /= camera.getScale();
                    mConfig->mProject.vpPos = mCamCoords - result;
                }

                auto scroll = Input::get()->getMouseWheel();
                if (scroll != 0)
                {
                    auto scalePos = pos / camera.getScale();

                    auto newVal = camera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
                    camera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
                    mConfig->mProject.zoomLevel = camera.getScale();

                    auto newPos = pos / camera.getScale();

                    auto offset = newPos - scalePos;
                    mConfig->mProject.vpPos = camera.getPosition() - offset;
                }

                camera.setPosition(mConfig->mProject.vpPos);
            }
        }
    }

    void CenterWindow::selectEntity(float x, float y)
    {
        auto& project = mConfig->mProject;

        if (!Input::get()->isCtrl) {
            if (Input::get()->onKeyPressed("MouseLeft"))
            {
                mMouseLastCoords = mConfig->mMouseCoords;
                Entity ent = { mConfig->mRender->mFramePicker.getEntId({ x, y }) };

                if (ent) {
                    project.mEnt = ent;
                    if (project.mEnt.hasComponent<TransformComponent>()) {
                        mEntLastPos = project.mEnt.getComponent<TransformComponent>()->getPosition();
                    }
                }
            }

            if (project.mEnt && Input::get()->onKeyDown("MouseLeft"))
            {
                if (project.mEnt.hasComponent<TransformComponent>()) {
                    auto trans = project.mEnt.getComponent<TransformComponent>();
                    Vec2f result = mConfig->mMouseCoords - mMouseLastCoords;
                    result /= mConfig->mRender->mCamera.getScale();

                    trans->x = mEntLastPos.x + result.x;
                    trans->y = mEntLastPos.y + result.y;
                }
            }
        }
    }

    void CenterWindow::showConfig()
    {
        auto camera = mConfig->mRender->mCamera;
        auto& project = mConfig->mProject;

        ImGui::SetNextWindowSize({ 0, 0 });
        auto winPos = ImGui::GetCursorScreenPos();
        ImVec2 pos(winPos.x + 200, winPos.y + 50);
        ImGui::SetNextWindowPos(pos);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoSavedSettings;

        if (ImGui::BeginPopupModal("Project Config", NULL, flags))
        {
            float width = 380 * 0.35f;
            ImGui::Text("Window Config");
            ImGui::Separator();
            ImGui::Row("Width", width);
            ImGui::DragInt("##win-width", &project.winWidth);
            ImGui::Row("Height", width);
            ImGui::DragInt("##win-height", &project.winHeight);

            ImGui::Separator();
            ImGui::Text("Viewport Config");
            ImGui::Separator();
            ImGui::Row("Width", width);
            ImGui::DragInt("##vp-width", &project.vpWidth);
            ImGui::Row("Height", width);
            ImGui::DragInt("##vp-height", &project.vpHeight);

            ImGui::Separator();
            ImGui::Text("Box2d Config");
            ImGui::Separator();

            ImGui::Row("Gravity", width);
            ImGui::Draw2Float("##gravity", project.gravity);

            ImGui::Row("Max FPS", width);
            ImGui::DragFloat("##timeIter", &project.maxFPS, 1, 16, 128, "%0.0f");

            ImGui::Row("Velocity Iter", width);
            ImGui::DragInt("##viter", &project.velIter, 1, 1, 20);

            ImGui::Row("Position Iter", width);
            ImGui::DragInt("##viter", &project.positionIter, 1, 1, 20);

            ImGui::Row("Auto Clear Force", width);
            ImGui::Checkbox("##cforce", &project.autoClearForce);
            ImGui::Separator();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
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

            Vec2f vpSize = mConfig->mRender->mCamera.getViewPortSize();

            auto winPos = ImGui::GetContentRegionAvail();
            auto pos = ImGui::GetCursorPos();

            Vec2f newSize((int)winPos.x, int(winPos.x / aspectRation));
            if (newSize.y > winPos.y)
            {
                newSize.y = winPos.y;
                newSize.x = winPos.y * aspectRation;
            }
            // add padding to the canvas
            newSize -= 10;
            //calculate the position centered oncanvas
            float x = roundf(std::max((winPos.x - newSize.x), 0.0f) * 0.5f);
            float y = roundf(std::max((winPos.y - newSize.y), 0.0f) * 0.5f);
            //set the new posotion
            ImGui::SetCursorPos({ x, y });

            if ((int)winSize.x != (int)newSize.x && (int)winSize.y != (int)newSize.y) {
                mConfig->mRender->resizeBuffers(newSize);
            }

            ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

            float localX = ImGui::GetIO().MousePos.x - canvas_pos.x;
            float localY = ImGui::GetIO().MousePos.y - canvas_pos.y;

            float xPos = mapIn(localX, 0, newSize.x, 0, vpSize.x);
            float yPos = vpSize.y - mapIn(localY, 0, newSize.y, 0, vpSize.y);

            // ImGui::Image((void*)mConfig->mRender->mFramePicker.getTextureId(), { newSize.x, newSize.y }, { 0, 1 }, { 1, 0 }, { 0,0,0,1 }, { 0.0, 0.0, 0.0, 1.0 });
            ImGui::Image((void*)framebuffer.getTextureId(), { newSize.x, newSize.y }, { 0, 1 }, { 1, 0 }, WHITE, { 0.0, 0.0, 0.0, 1.0 });
            if (mConfig->isHover = ImGui::IsItemHovered())
            {
                mConfig->mMouseCoords = { xPos, yPos };

                CameraControl();

                if (mConfig->state != Running)
                    selectEntity(localX, newSize.y - localY);
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
        static uint64_t saveStart;

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
                    if (saveStart) {
                        auto time = Timer::millis() - saveStart;

                        if (Timer::millis() - saveStart > 5000) {
                            saveStart = 0;
                        }
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.85f, 1.0f, 1.0f));
                        ImGui::Text("%llu - Save %s", time / 1000, currentScript.c_str());
                        ImGui::PopStyleColor();
                    }

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
                        mConfig->state = Editing;
                        mConfig->mRender->setScene(project.scene.get());
                        DebugRender::get()->setShouldDraw(mConfig->drawGrid);

                        mSysManager.stop();
                        mConfig->mTempProject.clearScene();
                    }
                    else {
                        mConfig->state = Running;

                        mConfig->mTempProject.Copy(mConfig->mProject);

                        DebugRender::get()->setShouldDraw(false);

                        mSysManager.init();
                        mConfig->mRender->setScene(mConfig->mTempProject.scene.get());
                    }
                }
                ImGui::SameLine();
                if (ImGui::TransparentButton(ICON_FA_COG)) {
                    ImGui::OpenPopup("Project Config");
                }
                showConfig();
            }
            else {
                auto scripts = AssetManager::get()->getAssets<Script>();
                static Script* script = nullptr;

                if (scripts.size() && currentScript.empty()) {
                    currentScript = scripts.begin()->first;
                    script = static_cast<Script*>(scripts[currentScript]);
                    mTextEditor.SetText(script->mBuffer);
                }


                if (ImGui::TransparentButton(ICON_FA_FILE " New")) {
                    ImGui::OpenPopup("New Script");
                }

                if (script) {
                    ImGui::SameLine();
                    if (ImGui::TransparentButton(ICON_FA_SAVE " Save")) {
                        script->save(mTextEditor.GetText().c_str());
                        saveStart = Timer::millis();
                    }

                    ImGui::SameLine();
                    ImGui::PushItemWidth(200);
                    if (ImGui::ComboBox("##scr-list", scripts, currentScript)) {
                        mTextEditor.SetText(static_cast<Script*>(scripts[currentScript])->mBuffer);
                    }
                    ImGui::PopItemWidth();

                    if (Input::get()->isCtrl && Input::get()->onKeyPressed("S")) {
                        script->save(mTextEditor.GetText().c_str());
                        saveStart = Timer::millis();
                    }
                }


                ImGui::SetNextWindowSize({ 280, 0 });
                static std::string name;
                if (ImGui::BeginPopup("New Script"))
                {
                    ImGui::Row("Name");
                    ImGui::InputText("##sc-name", &name);
                    ImGui::Separator();
                    ImGui::SetCursorPosX(100);
                    if (ImGui::Button("Create")) {
                        currentScript = name;
                        AssetManager::get()->addAsset<Script>(name, "assets/scripts/" + name);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancer")) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                else {
                    name = "";
                }
            }

        }
        ImGui::End();
    }
}