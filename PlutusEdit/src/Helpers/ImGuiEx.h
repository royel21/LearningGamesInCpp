#pragma once

#include <vector>
#include <imgui.h>
#include <iostream>
#include <unordered_map>

#include <entt.hpp>
#include <Core/type.h>
#include <ECS/Scene.h>
#include <Assets/AssetManager.h>

#include <Physics/Vectors.h>

#define IM_F4_2_I32COLOR(color) static_cast<int>(color[3] * 255) << 24 | \
                                static_cast<int>(color[2] * 255) << 16 | \
                                static_cast<int>(color[1] * 255) << 8 | \
                                static_cast<int>(color[0] * 255)

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

namespace Plutus
{
    inline bool compare(const glm::ivec2& a, const glm::ivec2& b)
    {
        if (a.x < b.x)
            return true;
        if (a.x > b.x)
            return false;
        if (a.y < b.y)
            return true;
        if (a.y > b.y)
            return false;
        return false;
    }

    template <typename T>
    inline bool hasVec(std::vector<T> items, int x, int y)
    {
        auto found = std::find_if(items.begin(), items.end(), [x, y](const glm::ivec2& m) -> bool
            { return m.x == x && m.y == y; });
        return found != items.end();
    }

} // namespace Plutus

namespace ImGui
{

    inline int checkLimit(int& v, int min, int max) {
        return v < min ? 0 : v > max ? max - 1 : v;
    }

    template <typename T>
    inline bool ComboBox(const char* label, const std::vector<T*>& data, int& selected) {
        std::string name;
        if (data.size()) {
            name = data[selected]->name;
        }

        bool isSelected = false;

        if (ImGui::BeginCombo(label, name.c_str()))
        {
            int i = 0;
            for (auto m : data)
            {
                bool is_selected = m->name.compare(data[selected]->name) == 0;
                if (ImGui::Selectable(m->name.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                i++;
            }
            ImGui::EndCombo();
        }

        return isSelected;
    }

    template <typename T>
    inline bool ComboBox(const char* label, const std::unordered_map<std::string, T>& data, std::string& selected)
    {
        bool isSelected = false;

        if (ImGui::BeginCombo(label, selected.c_str()))
        {
            for (auto m : data)
            {
                bool is_selected = m.first.compare(selected) == 0;
                if (ImGui::Selectable(m.first.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = m.first;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        return isSelected;
    }

    bool ComboBox(const char* label, const std::unordered_map<int, std::string>& data, int& selected);

    bool ComboBox(const char* label, const std::vector<std::string>& data, int& selected);

    bool TransparentButton(const char* label, ImVec4 color = { 1,1,1,1 });

    bool ListBox(const char* label, std::vector<Plutus::Entity*> data, Plutus::Entity* selected);

    bool Entities(const char* label, std::vector<Plutus::Ref<Plutus::Entity>>& entities, int& selected, int& remove);

    bool ColorInt(const char* label, unsigned int& color);

    void BeginDialog(const char* name, bool fixedPos = false);

    void EndDialog(bool& show);

    bool BeginUIGroup(ImGuiTableFlags flags = 0);
    void EndUIGroup();

    void BeginCol(const char* label, float width = -1);

    bool Draw2Float(char* id, glm::vec2& value, float step = 1.0f, const char* btntag1 = "X", const char* btntag2 = "Y");

    bool InputString(const char* label, std::string& text);

    bool Texture(Plutus::Texture* tileset, float scale, std::vector<glm::ivec3>& selected);

    bool DrawTextureOne(Plutus::Texture* tileset, int& selected);

    bool DrawTexture(Plutus::Texture* tileset, int winWidth = 0, int winHeight = 0, float scale = 1.0f, std::vector<glm::ivec3>* selected = nullptr, bool onlyOne = false);

    void DrawTexCoords(Plutus::Texture* tileset, glm::vec4& coords);
} // namespace ImGui