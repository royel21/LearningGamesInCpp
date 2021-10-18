#include "ConfigPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "../Config.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"
#include "../Helpers/ImGuiDialog.h"

#include "../Helpers/Render.h"

#include <glm/gtc/type_ptr.hpp>
#include <Graphics/DebugRenderer.h>

namespace Plutus
{
    void ConfigPanel::DrawConfigPanel() {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin("Config2");
        if (ImGui::BeginTabBar("##button window")) {
            if (ImGui::BeginTabItem("ViewPort"))
            {
                ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV;
                if (ImGui::BeginTable("ViewportControls", 2, flags)) {
                    ImGui::TableNextColumn();
                    if (ImGui::BeginUIGroup()) {
                        auto& camera = Render::get().mCamera;
                        auto csacle = camera.getScale();
                        ImGui::BeginCol("Zoom");
                        if (ImGui::InputFloat("##vp-Zoom", &csacle, 0.05f))
                        {
                            camera.setScale(CHECKLIMIT(csacle, 0.20f, 6.0f));
                        }
                        ImGui::BeginCol("Position");
                        auto pos = camera.getPosition();
                        if (ImGui::Draw2Float("#cpos", pos)) camera.setPosition(pos);
                        ImGui::EndUIGroup();
                    }
                    ImGui::TableNextColumn();
                    if (ImGui::BeginUIGroup()) {
                        static bool showGrid = true;
                        auto mDebugRender = DebugRender::get();
                        ImGui::BeginCol("Enable Grid");
                        if (ImGui::Checkbox("##Enable-grid", &showGrid))
                        {
                            mDebugRender->setShouldDraw(showGrid);
                        }
                        ImGui::BeginCol("Cell Size");
                        auto cellSize = mDebugRender->getCellSize();
                        if (ImGui::DragInt2("##Cell-Size", glm::value_ptr(cellSize)))
                            mDebugRender->setCellSize(cellSize);
                        static glm::vec4 color = mDebugRender->getGridLineColor();
                        ImGui::BeginCol("Line Color");
                        if (ImGui::ColorEdit3("###Grid-Color", glm::value_ptr(color)))
                        {
                            color.w = 1;
                            mDebugRender->setColor({ color });
                        }

                        ImGui::EndUIGroup();
                    }
                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Grid"))
            {

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
} // namespace Plutus
