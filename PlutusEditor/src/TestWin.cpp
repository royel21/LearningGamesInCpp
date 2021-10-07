#include "TestWin.h"
#include <imgui.h>
#include <string>

struct ImGuiExtensionStyle {
    float right_align = .35f; // 0-1
    float right_align_padding = 0.03f; // 0-1

    ImVec2 min_button_padding = { 48, 16 };
};

ImGuiExtensionStyle g_ext_style;

namespace ImGui
{


    void set_layout_for_known_width(const char* label, float width) {
        float right_offset = GetWindowContentRegionWidth() - width - g_ext_style.right_align_padding * GetWindowWidth();
        AlignTextToFramePadding();
        Text("%s", label);
        SameLine(right_offset);
    }

    void set_layout(const char* label, float offest = 0.03f) {
        float right_offset = GetWindowWidth() * 0.35f;
        AlignTextToFramePadding();
        Text("%s", label);

        SameLine(right_offset);
        float item_width = GetWindowWidth() - right_offset - GetWindowWidth() * offest;
        SetNextItemWidth(item_width);
    }


    bool RInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr) {
        set_layout(label, 0.165f);
        BeginGroup();
        bool ret = InputText("##id", buf, buf_size, flags, callback, user_data);
        SameLine();
        Button("Reset");
        EndGroup();
        return ret;
    }

    bool RInputFloat(const char* label, float* v, float p_step = 0.0f, float p_step_fast = 0.0f, const char* format = "", ImGuiInputTextFlags flags = 0) {

        set_layout(label);
        // SetNextItemWidth(CalcItemWidth() - 50.0f);
        InputFloat2("", v, format, flags);
        // SameLine();
        // Button("Reset", { 50.0f, 0 });
        return false;
    }

    bool RInputFloat2(const char* label, float v[2], const char* format, ImGuiInputTextFlags flags) {
        set_layout(label);
        PushID(v);
        bool ret = InputFloat2("", v, format, flags);
        PopID();
        return ret;
    }

    void drawTest()
    {
        Begin("Testing");
        set_layout("Testing");
        SetNextItemWidth(CalcItemWidth() - 50.0f);
        static float v = 0.35f;
        InputFloat("##val: ", &v, 0.1f);
        PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.0f,0 });
        SameLine();
        if (Button("Reset", { 50.0f, 0 })) {
            v = 0.35f;
        }
        PopStyleVar();
        End();
    }
} // namespace Plutus
