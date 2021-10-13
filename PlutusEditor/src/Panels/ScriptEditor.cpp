#include "ScriptEditor.h"
#include "../ImGuiEx.h"

#include <imgui.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>
#include "../IconsFontAwesome5.h"

namespace Plutus
{
    ScriptEditor::ScriptEditor()
    {

    };
    ScriptEditor::~ScriptEditor()
    {

    };

    void ScriptEditor::init() {
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

    void ScriptEditor::setScript(std::string script) {
        editor.SetText(script);
    }

    void ScriptEditor::drawUI() {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
        if (ImGui::Begin(ICON_FA_CODE" Script Editor", nullptr, flags)) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            auto& colors = ImGui::GetStyle().Colors;
            const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
            const auto& buttonActive = colors[ImGuiCol_ButtonActive];
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
            ImGui::Button(ICON_FA_FILE_ALT " New");
            ImGui::SameLine();
            ImGui::Button(ICON_FA_SAVE " Save");
            ImGui::SameLine();
            ImGui::Button(ICON_FA_FOLDER_OPEN "Load");
            ImGui::SameLine();

            if (scripts.size())
            {
                static int selected = 0;
                ImGui::ComboBox("##Script", scripts, selected);
                if (currentScript.compare(scripts[selected]) != 0) {
                    currentScript = scripts[selected];
                    editor.SetText(readFileAsString(currentScript.c_str()));
                }
            }
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);
            ImGui::Separator();
            editor.Render("TextEditor");
        }
        ImGui::End();
    }
} // namespace Plutus