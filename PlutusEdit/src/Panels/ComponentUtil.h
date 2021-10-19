#pragma once
#include "../Config.h"

#include <imgui.h> 
#include "../Helpers/IconsFontAwesome5.h"

namespace Plutus
{
    template<typename T>
    bool CollapseComponent(char* label, int id) {
        bool isOpen = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20);
        ImGui::PushID(id);
        if (ImGui::TransparentButton(ICON_FA_TRASH_ALT, false, { 1,0,0,1 })) {
            isOpen = false;
            Config::get().mProject->mEnt.removeComponent<T>();
        }
        ImGui::PopID();
        return isOpen;
    }
} // namespace Plutus

