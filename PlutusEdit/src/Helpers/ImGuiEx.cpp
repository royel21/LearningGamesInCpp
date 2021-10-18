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
        auto mInput = Plutus::Input::get();
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
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        return isSelected;
    }

    bool DrawTextureOne(Plutus::Texture* texture, int& selected) {
        std::vector<glm::ivec3> selecteds;
        if (DrawTexture(texture, 0, 0, 1.0f, &selecteds, true)) {
            selected = selecteds[0].z;
            return true;
        }
        return false;
    }

    bool DrawTexture(Plutus::Texture* texture, int winWidth, int winHeight, float scale, std::vector<glm::ivec3>* selected, bool onlyOne)
    {
        ImGui::BeginChild("##texture-map", { 0, (float)winHeight }, false, ImGuiWindowFlags_HorizontalScrollbar);
        auto mInput = Plutus::Input::get();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        auto size = ImGui::GetContentRegionAvail();

        ImVec2 cvPos = ImGui::GetCursorScreenPos();
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
                        if (onlyOne && i > 0) {
                            break;
                        }
                        ImVec2 start(drawSelect[i].x * tilWidth + cv_destStart.x, drawSelect[i].y * tilHeight + cv_destStart.y);
                        ImVec2 end(start.x + tilWidth, start.y + tilHeight);
                        drawList->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
                    }
                }
            }
        }
        ImGui::EndChild();
        // ImGui::PopStyleVar();
        return selected ? selected->size() : true;
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

    bool BeginUIGroup(ImGuiTableFlags flags) {
        static int id = 0;
        return ImGui::BeginTable(("##ground" + std::to_string(id)).c_str(), 2, flags);
    }

    void EndUIGroup() {
        ImGui::EndTable();
    }

    void BeginCol(const char* label, float width) {

        ImGui::TableNextColumn();
        PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.3f);
        const ImVec2 cursorPos = ImGui::GetCursorPos();
        // ImGui::SetCursorPos({ cursorPos.x + 4, cursorPos.y + 4 });
        ImGui::TextUnformatted(label);
        ImGui::PopItemWidth();
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(width);
    }


    bool LabelButton(const char* tag, ImVec2 size, int color = 0) {
        bool clicked = false;
        if (color == 1) {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.7f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.8f, 0.3f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.7f, 0.2f, 1.0f });
        }
        if (ImGui::Button(tag, size)) {
            clicked = true;
        }
        ImGui::PopStyleColor(3);
        return clicked;
    }

    bool Draw2Float(char* label, glm::vec2& value, float step, const char* btntag1, const char* btntag2) {
        bool changed = false;

        ImGui::BeginGroup();
        ImGui::PushID(label);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        glm::vec2 buttonSize(lineHeight + 3.0f, lineHeight);
        float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 2.0f) / 2.0f;

        ImGui::PushItemWidth(widthEach);
        if (LabelButton(btntag1, buttonSize)) {
            value.x = 0;
            changed = true;
        }
        ImGui::SameLine();

        float vecValuesX = value.x;
        if (ImGui::DragFloat("##x", &vecValuesX, step, 0, 0, "%0.1f")) changed = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushItemWidth(widthEach);
        if (LabelButton(btntag2, buttonSize, 1)) {
            value.y = 0;
            changed = true;
        }

        ImGui::SameLine();
        float vecValuesY = value.y;
        if (ImGui::DragFloat("##y", &vecValuesY, step, 0, 0, "%0.1f")) changed = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        value.x = vecValuesX;
        value.y = vecValuesY;

        ImGui::PopStyleVar();
        ImGui::EndGroup();
        ImGui::PopID();
        return changed;
    }

    bool InputString(const char* label, std::string& text) {
        char buffer[128] = { 0 };
        strncpy_s(buffer, text.c_str(), text.length());
        if (ImGui::InputText(label, buffer, sizeof(buffer)))
        {
            text = std::string(buffer);
            return true;
        }
        return false;
    }

    const uint32_t color1 = IM_COL32(50, 50, 50, 255);
    const uint32_t color2 = IM_COL32(50, 50, 60, 255);
    const uint32_t color3 = IM_COL32(60, 60, 70, 255);

    void DrawTexCoords(Plutus::Texture* tileset, glm::vec4& coords) {
        auto mInput = Plutus::Input::get();
        const int w = tileset->texWidth;
        const int h = tileset->texHeight;
        uint32_t id = tileset->texId;

        static float scale = 1;
        static ImVector<ImVec2> points;
        if (ImGui::Button(ICON_FA_ADJUST " Use") && points.Size > 1)
        {
            coords.x = (points[0].x / scale) / w;
            coords.y = (points[0].y / scale) / h;
            coords.z = (points[1].x / scale) / w;
            coords.w = (points[1].y / scale) / h;
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_BRUSH " Clear"))
        {
            points.clear();
        }
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        if (ImGui::DragFloat("Scale##cv", &scale, 0.01f, 0.4f, 5)) {
            points.clear();
        }
        ImGui::PopItemWidth();
        ImGui::Separator();
        auto size = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("##draw-cv", size, false, ImGuiWindowFlags_HorizontalScrollbar); {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
            ImVec2 cv_destStart(cvPos.x, cvPos.y);
            ImVec2 cvDestEnd(cvPos.x + w * scale, cvPos.y + h * scale);
            ImGui::Image((void*)id, ImVec2(w * scale, h * scale));
            {
                static ImVec2 StartCoords(0, 0);
                static ImVec2 EndCoords(0, 0);
                static bool firstClick = false;
                static bool SecondClick = false;

                static bool mDown;
                drawList->AddRectFilledMultiColor(cvPos, cvDestEnd, color1, color2, color3, color2);

                auto color = IM_COL32(255, 255, 255, 100);
                if (id)
                {
                    drawList->AddImage((void*)id, cvPos, cvDestEnd);
                    drawList->AddRect(cvPos, cvDestEnd, color);
                }
                if (ImGui::IsItemHovered())
                {
                    ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                    if (mInput->onKeyDown("MouseLeft"))
                    {
                        if (!mDown)
                        {
                            StartCoords = ImVec2(mpos_in_canvas.x, mpos_in_canvas.y);
                            mDown = true;
                        }
                        drawList->AddRect(ImVec2(cvPos.x + StartCoords.x, cvPos.y + StartCoords.y),
                            ImVec2(cvPos.x + mpos_in_canvas.x, cvPos.y + mpos_in_canvas.y), IM_COL32(255, 255, 255, 255));
                        if (points.Size > 1) {
                            points.clear();
                        }
                    }

                    if (!mInput->onKeyDown("MouseLeft"))
                    {
                        if (mDown)
                        {
                            EndCoords = ImVec2(mpos_in_canvas.x, mpos_in_canvas.y);
                            points.push_back(StartCoords);
                            points.push_back(EndCoords);
                            mDown = false;
                        }
                    }

                    if (mInput->onKeyPressed("MouseRight"))
                    {
                        if (points.size())
                        {
                            points.pop_back();
                            points.pop_back();
                        }
                    }
                }
                else
                {
                    mDown = false;
                }
                if (points.Size > 1) {
                    drawList->AddRect(
                        ImVec2(cvPos.x + points[0].x, cvPos.y + points[0].y),
                        ImVec2(cvPos.x + points[1].x, cvPos.y + points[1].y),
                        IM_COL32(255, 255, 255, 255));
                }

            }
        }
        ImGui::EndChild();
    }
}