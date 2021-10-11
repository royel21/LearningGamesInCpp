#pragma once
#include <string>
#include <vector>

#include "./ImGuiColorTextEdit/TextEditor.h"

namespace Plutus
{
    class ScriptEditor
    {
    public:
        ScriptEditor();
        ~ScriptEditor();
        void init();
        void drawUI();
        void setScript(std::string script);
    private:
        std::string currentScript;
        std::vector<std::string> scripts;
        TextEditor editor;
    };
} // namespace Plutus