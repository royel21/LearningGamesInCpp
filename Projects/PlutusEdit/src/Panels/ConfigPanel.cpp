#include "ConfigPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "../Config.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"
#include "../Helpers/ImGuiDialog.h"

#include "../Helpers/Render.h"

#include <Graphics/DebugRenderer.h>

namespace Plutus
{
    void ConfigPanel::DrawConfigPanel() {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
        if (ImGui::Begin("Config2")) {
            if (ImGui::BeginTabBar("##button window")) {
                if (ImGui::BeginTabItem("ViewPort"))
                {
                    ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV;
                    if (ImGui::BeginTable("ViewportControls", 2, flags)) {
                        ImGui::TableNextColumn();
                        if (ImGui::BeginUIGroup()) {

                            auto& camera = Render::get().mCamera;
                            auto& zoom = Config::get().vpZoom;
                            ImGui::BeginCol("Zoom");
                            if (ImGui::InputFloat("##vp-Zoom", &zoom, 0.05f))
                            {
                                zoom = CHECKLIMIT(zoom, 0.20f, 10.0f);
                                camera.setScale(zoom);
                            }
                            ImGui::BeginCol("Position");
                            if (ImGui::Draw2Float("#cpos", Config::get().vpPos))
                                camera.setPosition(Config::get().vpPos);

                            ImGui::BeginCol("BG Color");
                            ImGui::ColorEdit4("#vp-color", &Config::get().vpColor.x);
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
                            if (ImGui::DragInt2("##Cell-Size", &cellSize.x))
                                mDebugRender->setCellSize(cellSize);
                            static vec4f color = mDebugRender->getGridLineColor();
                            ImGui::BeginCol("Line Color");
                            if (ImGui::ColorEdit3("###Grid-Color", &color.x))
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
        }
        ImGui::End();
    }
} // namespace Plutus
