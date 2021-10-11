#include "ScriptEditor.h"
#include "../ImGuiEx.h"

#include <imgui.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>


namespace Plutus
{
    ScriptEditor::ScriptEditor()
    {

    };
    ScriptEditor::~ScriptEditor()
    {

    };

    void ScriptEditor::init() {
        editor.SetPalette(TextEditor::GetDarkPalette());
        auto lang = TextEditor::LanguageDefinition::Lua();
        editor.SetLanguageDefinition(lang);


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

        if (ImGui::Begin("Script Editor")) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            auto& colors = ImGui::GetStyle().Colors;
            const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
            const auto& buttonActive = colors[ImGuiCol_ButtonActive];
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
            ImGui::Button("New");
            ImGui::SameLine();
            ImGui::Button("Save");
            ImGui::SameLine();
            ImGui::Button("Load");
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