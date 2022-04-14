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
                        {
                            auto width = ImGui::GetContentRegionAvailWidth() * 0.3f;
                            auto& camera = Render::get().mCamera;
                            auto& zoom = Config::get().vpZoom;
                            ImGui::Row("Zoom", width);
                            if (ImGui::DragFloat("##vp-Zoom", &zoom, 0.05f))
                            {
                                zoom = CHECKLIMIT(zoom, 0.20f, 10.0f);
                                camera.setScale(zoom);
                            }
                            ImGui::Row("Position", width);
                            if (ImGui::Draw2Float("##cpos", Config::get().vpPos))
                                camera.setPosition(Config::get().vpPos);

                            ImGui::Row("BG Color", width);
                            ImGui::ColorEdit4("##vp-color", &Config::get().vpColor.x);
                        }

                        ImGui::TableNextColumn();
                        {
                            auto width = ImGui::GetContentRegionAvailWidth() * 0.3f;
                            static bool showGrid = true;
                            auto mDebugRender = DebugRender::get();
                            ImGui::Row("Enable Grid", width);
                            if (ImGui::Checkbox("##Enable-grid", &showGrid))
                            {
                                mDebugRender->setShouldDraw(showGrid);
                            }
                            ImGui::Row("Cell Size", width);
                            auto cellSize = mDebugRender->getCellSize();
                            if (ImGui::DragInt2("##Cell-Size", &cellSize.x))
                                mDebugRender->setCellSize(cellSize);
                            static vec4f color = mDebugRender->getGridLineColor();
                            ImGui::Row("Line Color", width);
                            if (ImGui::ColorEdit3("##Grid-Color", &color.x))
                            {
                                color.w = 1;
                                mDebugRender->setColor({ color });
                            }
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
