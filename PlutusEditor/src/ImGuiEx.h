#pragma once
#include <vector>
#include <unordered_map>
#include "imgui.h"
#include "iostream"
#include "entt.hpp"
#include "ECS/Scene.h"
#include "Assets/Textures.h"
#include "Input/Input.h"
#include "IconsFontAwesome5.h"
#include "ECS/Components.h"
#include <Core/type.h>

#define IM_F4_2_I32COLOR(color) static_cast<int>(color[3] * 255) << 24 | static_cast<int>(color[2] * 255) << 16 | static_cast<int>(color[1] * 255) << 8 | static_cast<int>(color[0] * 255)

namespace Plutus
{
    inline bool compare(const glm::ivec2 &a, const glm::ivec2 &b)
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
        auto found = std::find_if(items.begin(), items.end(), [x, y](const glm::ivec2 &m) -> bool
                                  { return m.x == x && m.y == y; });
        return found != items.end();
    }
} // namespace Plutus

namespace ImGui
{
    template <typename T>
    inline bool ComboBox(const char *label, const std::vector<T *> &data, int &selected)
    {
        bool isSelected = false;

        if (ImGui::BeginCombo(label, data[selected]->name.c_str()))
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

    inline bool ComboBox(const char *label, const std::vector<std::string> &data, int &selected)
    {
        bool isSelected = false;

        if (ImGui::BeginCombo(label, data[selected].c_str()))
        {
            int i = 0;
            for (auto m : data)
            {
                bool is_selected = m.compare(data[selected]) == 0;
                if (ImGui::Selectable(m.c_str(), is_selected))
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
    inline bool ComboBox(const char *label, const std::unordered_map<std::string, T> &data, std::string &selected)
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

    inline bool ListBox(const char *label, std::vector<Plutus::Entity *> data, Plutus::Entity *selected)
    {
        bool isSelected = false;
        if (ImGui::ListBoxHeader(label, data.size()))
        {
            for (auto ent : data)
            {

                bool is_selected = ent->getName().compare(selected->getName()) == 0;

                if (ImGui::Selectable(selected->getName().c_str(), is_selected))
                {
                    isSelected = true;
                    selected = ent;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::ListBoxFooter();
        }
        return isSelected;
    }

    inline bool Entities(const char *label, std::vector<Plutus::Ref<Plutus::Entity>> &entities, int &selected, int &remove)
    {
        bool isSelected = false;
        for (uint32_t i = 0; i < entities.size(); i++)
        {
            bool is_selected = i == selected;
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
            if (ImGui::Button(ICON_FA_TRASH_ALT " ##remove"))
            {
                remove = i;
            }
            ImGui::PopID();
            ImGui::PopStyleColor();
            ImGui::SameLine();
            if (ImGui::Selectable(entities[i]->getName().c_str(), is_selected))
            {
                isSelected = true;
                selected = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        return isSelected;
    }

    inline bool Texure(Plutus::Texure *tileset, float scale, std::vector<glm::ivec3> &selected)
    {
        bool isSelected = false;
        auto mInput = Plutus::Input::getInstance();
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        auto size = ImGui::GetContentRegionAvail();

        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);
        const int w = tileset->texWidth;
        const int h = tileset->texHeight;

        ImVec2 cvDestEnd(cvPos.x + w * scale, cvPos.y + h * scale);
        ImGui::Image((void *)tileset->texId, ImVec2(w * scale, h * scale));
        {
            auto color = IM_COL32(255, 255, 255, 100);
            if (tileset->texId)
            {
                drawList->AddRect(cvPos, cvDestEnd, color);
            }

            if (tileset->tileWidth && tileset->tileHeight)
            {
                float tileWidth = tileset->tileWidth * scale;
                float tileHeight = tileset->tileHeight * scale;

                float textureHeight = h * scale;
                float textureWidth = w * scale;
                int columns = static_cast<int>(textureWidth / tileWidth);
                if (tileWidth)
                {
                    for (float y = 0; y < textureHeight; y += tileHeight)
                    {
                        drawList->AddLine(ImVec2(cvPos.x, cvPos.y + y),
                                          ImVec2(cvDestEnd.x, cvPos.y + y), color, 1.0f);
                    }
                    for (float x = 0; x < textureWidth; x += tileWidth)
                    {
                        drawList->AddLine(ImVec2(cvPos.x + x, cvPos.y),
                                          ImVec2(cvPos.x + x, cvDestEnd.y), color, 1.0f);
                    }
                }
                //Rect
                static std::vector<glm::ivec2> sels;
                static std::vector<glm::ivec2> drawSelect;

                static bool mDown = false;
                if (ImGui::IsItemHovered())
                {
                    ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                    float x = floor(mpos_in_canvas.x / tileWidth);
                    float y = floor(mpos_in_canvas.y / tileHeight);
                    ImVec2 start(x * tileWidth + cvPos.x, y * tileHeight + cvPos.y);
                    ImVec2 end(start.x + tileWidth, start.y + tileHeight);

                    drawList->AddRect(start, end, IM_COL32(255, 0, 0, 255));

                    if (mInput->onKeyPressed("MouseLeft"))
                    {
                        mDown = true;
                        sels.clear();
                        selected.clear();
                        drawSelect.clear();
                    }

                    if (!mInput->onKeyDown("MouseLeft"))
                    {
                        mDown = false;
                    }

                    if (mDown)
                    {
                        if (!Plutus::hasVec(sels, static_cast<int>(x), static_cast<int>(y)))
                        {
                            sels.emplace_back(x, y);

                            isSelected = true;
                            if (sels.size())
                            {
                                std::sort(sels.begin(), sels.end(), Plutus::compare);
                                auto first = sels.front();
                                auto last = sels.back();
                                selected.clear();
                                int i = 0;
                                for (int xPos = first.x; xPos <= last.x; xPos++)
                                {
                                    int i2 = 0;
                                    for (int yPos = first.y; yPos <= last.y; yPos++)
                                    {
                                        selected.emplace_back(i, i2++, xPos + yPos * columns);
                                        if (!Plutus::hasVec(drawSelect, xPos, yPos))
                                            drawSelect.emplace_back(xPos, yPos);
                                    }
                                    i++;
                                }
                                if (i == 3)
                                {
                                    i = 0;
                                }
                            }
                        }
                    }

                    if (mInput->onKeyDown("MouseRight"))
                    {
                        drawSelect.clear();
                        selected.clear();
                    }
                }

                for (uint32_t i = 0; i < drawSelect.size(); i++)
                {
                    ImVec2 start(drawSelect[i].x * tileWidth + cv_destStart.x, drawSelect[i].y * tileHeight + cv_destStart.y);
                    ImVec2 end(start.x + tileWidth, start.y + tileHeight);
                    drawList->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
                }
            }
            return isSelected;
        }
    }

    inline bool ColorInt(const char *label, unsigned int &color)
    {
        bool change = false;
        int r = color & 255;
        int g = (color >> 8) & 255;
        int b = (color >> 16) & 255;
        int a = (color >> 24) & 255;

        float mcolor[] = {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
        if (ImGui::ColorEdit4(label, mcolor))
        {
            color = IM_F4_2_I32COLOR(mcolor);
            change = true;
        }
        return change;
    }

    inline void BeginDialog(const char *name, float width, float height)
    {
        auto pos = ImGui::GetWindowPos();
        ImGui::SetNextWindowPos({pos.x + 5, pos.y + 60});

        ImGui::SetNextWindowSize({width, height});
        ImGui::OpenPopup(name);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
        ImGui::BeginPopupModal(name, NULL, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
    }

    inline void EndDialog(bool &show)
    {
        auto size = ImGui::GetWindowSize();
        ImGui::SetCursorPos({size.x - 60.0f, size.y - 35.0f});
        if (ImGui::Button("Cancel##modal-1"))
            show = false;
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }
} // namespace ImGui