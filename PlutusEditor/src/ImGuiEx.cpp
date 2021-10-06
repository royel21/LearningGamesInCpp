#include "ImGuiEx.h"
#include "IconsFontAwesome5.h"

#include <Input/Input.h>
#include <ECS/Components.h>

namespace ImGui {
    bool TransparentButton(char* label, ImVec4 color) {
        bool isActive = false;
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
        if (ImGui::Button(label)) {
            isActive = true;
        }
        ImGui::PopStyleColor(3);
        return isActive;
    }

    bool ComboBox(const char* label, const std::vector<std::string>& data, int& selected)
    {
        bool isSelected = false;
        std::string name;

        selected = checkLimit(selected, 0, (int)data.size() - 1);

        if (data.size()) {
            name = data[selected];
        }

        if (ImGui::BeginCombo(label, name.c_str()))
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

    bool ListBox(const char* label, std::vector<Plutus::Entity*> data, Plutus::Entity* selected)
    {
        bool isSelected = false;
        if (ImGui::ListBoxHeader(label, (int)data.size()))
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

    bool Entities(const char* label, std::vector<Plutus::Ref<Plutus::Entity>>& entities, int& selected, int& remove)
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

    bool Texture(Plutus::Texture* tileset, float scale, std::vector<glm::ivec3>& selected)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("##texture-map", { 0,0 }, true, ImGuiWindowFlags_HorizontalScrollbar);
        bool isSelected = false;
        auto mInput = Plutus::Input::getInstance();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        auto size = ImGui::GetContentRegionAvail();

        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);
        const int w = tileset->texWidth;
        const int h = tileset->texHeight;

        ImVec2 cvDestEnd(cvPos.x + w * scale, cvPos.y + h * scale);
        ImGui::Image((void*)tileset->texId, ImVec2(w * scale, h * scale));
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
            ImGui::EndChild();
            ImGui::PopStyleVar();
            return isSelected;
        }
    }

    bool DrawTexture(Plutus::Texture* texture, int winWidth, int winHeight, std::vector<glm::ivec3>* selected)
    {
        static float scale = 1.0f;
        ImGui::DragFloat("Scale##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("##texture-map", { (float)winWidth, (float)winHeight }, true, ImGuiWindowFlags_HorizontalScrollbar);
        bool isSelected = false;
        auto mInput = Plutus::Input::getInstance();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        auto size = ImGui::GetContentRegionAvail();

        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);
        const int w = texture->texWidth;
        const int h = texture->texHeight;

        ImVec2 cvDestEnd(cvPos.x + w * scale, cvPos.y + h * scale);
        ImGui::Image((void*)texture->texId, ImVec2(w * scale, h * scale));
        {
            auto color = IM_COL32(255, 255, 255, 100);
            if (texture->texId)
            {
                drawList->AddRect(cvPos, cvDestEnd, color);
            }

            if (texture->tileWidth && texture->tileHeight)
            {
                float tilWidth = texture->tileWidth * scale;
                float tilHeight = texture->tileHeight * scale;

                float textureHeight = h * scale;
                float textureWidth = w * scale;
                int columns = static_cast<int>(textureWidth / tilWidth);
                for (float y = 0; y < textureHeight; y += tilHeight)
                {
                    drawList->AddLine(ImVec2(cvPos.x, cvPos.y + y),
                        ImVec2(cvDestEnd.x, cvPos.y + y), color, 1.0f);
                }
                for (float x = 0; x < textureWidth; x += tilWidth)
                {
                    drawList->AddLine(ImVec2(cvPos.x + x, cvPos.y),
                        ImVec2(cvPos.x + x, cvDestEnd.y), color, 1.0f);
                }

                if (selected) {
                    static std::vector<glm::ivec2> sels;
                    static std::vector<glm::ivec2> drawSelect;

                    static bool mDown = false;
                    if (ImGui::IsItemHovered())
                    {
                        ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                        float x = floor(mpos_in_canvas.x / tilWidth);
                        float y = floor(mpos_in_canvas.y / tilHeight);
                        ImVec2 start(x * tilWidth + cvPos.x, y * tilHeight + cvPos.y);
                        ImVec2 end(start.x + tilWidth, start.y + tilHeight);

                        drawList->AddRect(start, end, IM_COL32(255, 0, 0, 255));

                        if (mInput->onKeyPressed("MouseLeft"))
                        {
                            mDown = true;
                            sels.clear();
                            selected->clear();
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
                                    selected->clear();
                                    int i = 0;
                                    for (int xPos = first.x; xPos <= last.x; xPos++)
                                    {
                                        int i2 = 0;
                                        for (int yPos = first.y; yPos <= last.y; yPos++)
                                        {
                                            selected->emplace_back(i, i2++, xPos + yPos * columns);
                                            if (!Plutus::hasVec(drawSelect, xPos, yPos))
                                                drawSelect.emplace_back(xPos, yPos);
                                        }
                                        i++;
                                    }
                                }
                            }
                        }

                        if (mInput->onKeyDown("MouseRight"))
                        {
                            drawSelect.clear();
                            selected->clear();
                        }
                    }

                    for (uint32_t i = 0; i < drawSelect.size(); i++)
                    {
                        ImVec2 start(drawSelect[i].x * tilWidth + cv_destStart.x, drawSelect[i].y * tilHeight + cv_destStart.y);
                        ImVec2 end(start.x + tilWidth, start.y + tilHeight);
                        drawList->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
                    }
                }
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();
            return true;
        }
    }


    bool ColorInt(const char* label, unsigned int& color)
    {
        bool change = false;
        int r = color & 255;
        int g = (color >> 8) & 255;
        int b = (color >> 16) & 255;
        int a = (color >> 24) & 255;

        float mcolor[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
        if (ImGui::ColorEdit4(label, mcolor))
        {
            color = IM_F4_2_I32COLOR(mcolor);
            change = true;
        }
        return change;
    }

    void BeginDialog(const char* name, float width, float height, bool fixedPos)
    {
        if (fixedPos) {
            auto pos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos({ pos.x + 5, pos.y + 60 });
        }

        ImGui::OpenPopup(name);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::BeginPopupModal(name, NULL, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
    }

    void EndDialog(bool& show)
    {
        ImGui::SameLine();
        if (ImGui::Button("Cancel##modal-1"))
            show = false;
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }

}