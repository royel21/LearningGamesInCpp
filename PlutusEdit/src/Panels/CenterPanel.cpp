#include "CenterPanel.h"
#include <imgui.h>
#include <imgui_internal.h>

#include <Input/Input.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>

#include "../Context.h"
#include "../Helpers/Render.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

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
    }


    void CenterPanel::DrawViewPortControls()
    {
        if (Context.mVpState.isHover) {
            auto pos = Context.mVpState.mouseCoords;
            auto& camera = Render::get().mCamera;

            if (Input::get()->onKeyPressed("R"))
                camera.setPosition(0, 0);

            if (Input::get()->onKeyPressed("Z"))
                camera.setScale(1);

            if (Input::get()->onKeyPressed("MouseLeft"))
            {
                mMouseLastCoords = pos;
                auto cPos = camera.getPosition();
                mCamCoords = { cPos.x, cPos.y };
            }
            // move the camera
            if (Input::get()->isCtrl)
            {
                if (Input::get()->onKeyDown("MouseLeft"))
                {
                    vec2f result = pos - mMouseLastCoords;
                    result /= camera.getScale();
                    auto newPos = mCamCoords - result;
                    camera.setPosition({ newPos.x, newPos.y });
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

            glm::vec2 newSize(winPos.x, winPos.x / aspectRation);
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
            if (Context.mVpState.isHover = ImGui::IsItemHovered())
            {
                Context.mVpState.mouseCoords = { xPos, yPos };

                DrawViewPortControls();
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(1);
    }

    void CenterPanel::DrawCenterPanel()
    {
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
            if (ImGui::BeginTabItem("Script Editor")) {
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
            ImGui::TransparentButton(ICON_FA_PLAY);
            ImGui::SameLine();
            ImGui::TransparentButton(ICON_FA_STOP);
        }
        else {
            ImGui::TransparentButton(ICON_FA_FILE " New");
            ImGui::SameLine();
            ImGui::TransparentButton(ICON_FA_SAVE " Save");
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