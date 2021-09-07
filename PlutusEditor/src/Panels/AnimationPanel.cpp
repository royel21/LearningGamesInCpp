#include "AnimationPanel.h"
#include <ECS/Scene.h>
#include <imgui.h>
#include "ImGuiEx.h"
#include <Assets/AssetManager.h>

#define ZeroMem(arr, val) std::memset(arr, val, sizeof(arr))

namespace Plutus
{
    void AnimationPanel::draw(Entity* ent)
    {
        if (ImGui::CollapsingHeader("Animation##comp"))
        {
            static bool newAnin = false;

            auto& animate = ent->getComponent<Animation>();
            //Animations
            auto& sequences = animate.mSequences;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
            if (ImGui::Button(ICON_FA_PLUS_CIRCLE "##add-anin"))
            {
                newAnin = true;
                mSeq = {};
                ZeroMem(name, 0);
            }

            ImGui::SameLine();
            auto it = sequences.begin();
            static std::string selected = it != sequences.end() ? it->first : "";
            ImGui::PushItemWidth(100);
            ImGui::ComboBox("Sequences", sequences, selected);

            static auto textures = Plutus::AssetManager::get()->mTextures.mTileSets;
            static std::string selectedTex;
            if (ImGui::Button(ICON_FA_PLUS_CIRCLE "##add-tex"))
            {
                newAnin = true;
                mSeq = {};
                ZeroMem(name, 0);
            }
            ImGui::SameLine();
            ImGui::ComboBox("Textures", textures, selectedTex);
            ImGui::PopItemWidth();
            ImGui::PopStyleColor();

            if (!selected.empty())
            {
                auto& anim = sequences[selected];
                //Textures
                // auto tilesets = mAManager->getTilesets();
                // ImGui::ComboBox<Texture>("TileSheet", tilesets, );
            }
            SequenceEditor(newAnin, animate);
        }
    }

    void AnimationPanel::SequenceEditor(bool& show, Animation& anim)
    {
        if (show)
        {
            static int textureIndex = 0;
            static char seqName[128];
            static std::vector<int> seq;

            ImGui::SetNextWindowSize({ 450.0f, 350.0f });
            if (ImGui::Begin("New Sequence", &show))
            {
                ImGui::InputText("Name##seq", seqName, IM_ARRAYSIZE(seqName));
                auto& textures = anim.mTextures;
                if (textures.size())
                {
                    ImGui::ComboBox<Texture>("TileSheet", textures, textureIndex);
                }
                ImGui::End();
            }
            else
            {
                textureIndex = 0;
            }
        }
    }

} // namespace Plutus

/*
ImGui::BeginDialog("New Sequences", 280.0f, 200.0f);
ImGui::InputText("Name##anim", name, IM_ARRAYSIZE(name));

ImGui::SetCursorPos({180.0f, 200.0f - 35.0f});
if (ImGui::Button("Save"))
{
    // animate.AddSequences(name);
    show = false;
}
ImGui::EndDialog(show);

*/