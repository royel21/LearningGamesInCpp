#include "ImGuiDialog.h"
#include <imgui.h>

#include "ImGuiEx.h"


namespace ImGui
{
    bool NewFileDialig(const char* label, std::function<void(const std::string&)> callback)
    {
        static std::string name;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::OpenPopup(label);
        if (ImGui::BeginPopupModal(label, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::BeginUIGroup();
            ImGui::BeginCol("Name", 200);
            ImGui::InputString("##n-p", name);
            ImGui::EndUIGroup();
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                callback(name);
                name = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel##modal-1")) {
                callback("");
                name = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }
        return true;
    }
}