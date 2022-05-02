#include "ImGuiDialog.h"
#include <imgui.h>

#include "ImGuiEx.h"


namespace ImGui
{
    bool NewFileDialig(const char* label, std::function<void(const std::string&)> callback)
    {
        ImVec2 btnWidth = { 75, 0 };
        static std::string name;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize({ 300,0 });
        ImGui::OpenPopup(label);
        if (ImGui::BeginPopupModal(label, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::Row("Name");
            ImGui::InputString("##n-p", name);

            ImGui::Separator();
            ImGui::InvisibleButton("##n-name", { 1,1 });
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
            ImGui::SameLine(70);
            if (ImGui::Button("OK", btnWidth)) {
                callback(name);
                name = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel##modal-1", btnWidth)) {
                callback("");
                name = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }
        return true;
    }

    bool BeginDialogText(const char* label) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::OpenPopup(label);
        return ImGui::BeginPopupModal(label, NULL, ImGuiWindowFlags_AlwaysAutoResize);
    }

    bool EndDialogText(std::function<void(bool)> saveCallBack) {
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            saveCallBack(true);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##modal-1")) {
            saveCallBack(false);
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();
        ImGui::EndPopup();
        return true;
    }
}