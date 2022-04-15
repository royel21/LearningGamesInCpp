#pragma once
#include <Config.h>

#include <vector>
#include <imgui.h> 
#include <algorithm>

#include <Config.h>
#include <Helpers/Render.h>
#include <Helpers/ImGuiEx.h>
#include <Helpers/IconsFontAwesome5.h>
#include <functional>

namespace Plutus
{
    template<typename T>
    bool CollapseComponent(char* label, int id, Config* config) {
        bool isOpen = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 20);
        ImGui::PushID(id);
        if (ImGui::TransparentButton(ICON_FA_TRASH_ALT, false, { 1,0,0,1 })) {
            isOpen = false;
            config->mProject->mEnt.removeComponent<T>();
        }
        ImGui::PopID();
        return isOpen;
    }

    template<typename T>
    void removeItem(std::vector<T>& vec, T* item) {
        auto it = std::find_if(vec.begin(), vec.end(), [&](T& e) {
            return &e == item;
            });

        if (it != vec.end()) {
            vec.erase(e);
        }
    }

} // namespace Plutus

