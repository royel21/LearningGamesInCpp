#include "BottomWindow.h"

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
    void BottomWindow::draw() {
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
                            auto& camera = mRender->mCamera;
                            auto& zoom = mConfig->vpZoom;
                            ImGui::Row("Zoom", width);

                            ImGui::BeginGroup();
                            ImGui::PushID("zoom-group");
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 4, 0 });
                            if (ImGui::InputFloat("##seq-time", &zoom, 0.05f))
                            {
                                mConfig->vpZoom = CHECKLIMIT(zoom, 0.20f, 10.0f);
                                camera.setScale(mConfig->vpZoom);
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("RS", { 24, 0 })) {
                                camera.setScale(1.0f);
                                mConfig->vpZoom = 1.0f;
                            }
                            ImGui::PopStyleVar();
                            ImGui::PopID();
                            ImGui::EndGroup();

                            ImGui::Row("Position", width);
                            if (ImGui::Draw2Float("##cpos", mConfig->vpPos))
                                camera.setPosition(mConfig->vpPos);

                            ImGui::Row("BG Color", width);
                            ImGui::ColorEdit4("##vp-color", &mConfig->vpColor.x);
                        }

                        ImGui::TableNextColumn();
                        {
                            auto width = ImGui::GetContentRegionAvailWidth() * 0.3f;

                            auto mDebugRender = DebugRender::get();

                            ImGui::Row("Enable Grid", width);
                            if (ImGui::Checkbox("##Enable-grid", &mConfig->drawGrid))
                            {
                                mDebugRender->setShouldDraw(mConfig->drawGrid);
                            }

                            auto cellSize = mDebugRender->getCellSize();
                            ImGui::Row("Cell Size", width);
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
