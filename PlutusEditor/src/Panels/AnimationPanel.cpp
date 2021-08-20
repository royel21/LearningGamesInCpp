#include "AnimationPanel.h"
#include <ECS/Scene.h>
#include <imgui.h>
#include "ImGuiEx.h"

#define ZeroMem(arr, val) std::memset(arr, val, sizeof(arr))

namespace Plutus
{
    void AnimationPanel::draw(Entity *ent)
    {
        if (ImGui::CollapsingHeader("Animation##comp"))
        {
            static bool newAnin = false;

            auto &animate = ent->getComponent<Animation>();
            //Animations
            auto &sequences = animate.mSequences;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
            if (ImGui::Button(ICON_FA_PLUS_CIRCLE "##add-anin"))
            {
                newAnin = true;
                mSeq = {};
                ZeroMem(name, 0);
            }
            ImGui::PopStyleColor();

            ImGui::SameLine();
            auto it = sequences.begin();
            static std::string selected = it != sequences.end() ? it->first : "";
            ImGui::PushItemWidth(100);
            ImGui::ComboBox("Sequences", sequences, selected);
            ImGui::PopItemWidth();

            if (!selected.empty())
            {
                auto &anim = sequences[selected];
                //Textures
                // auto tilesets = mAManager->getTilesets();
                // ImGui::ComboBox<TileSet>("TileSheet", tilesets, );
            }
            SequenceEditor(newAnin);
        }
    }

    void AnimationPanel::SequenceEditor(bool &show)
    {
        if (show)
        {
            ImGui::BeginDialog("New Sequences", 280.0f, 200.0f);
            ImGui::InputText("Name##anim", name, IM_ARRAYSIZE(name));

            ImGui::SetCursorPos({180.0f, 200.0f - 35.0f});
            if (ImGui::Button("Save"))
            {
                // animate.AddSequences(name);
                show = false;
            }
            ImGui::EndDialog(show);
        }
    }

} // namespace Plutus
