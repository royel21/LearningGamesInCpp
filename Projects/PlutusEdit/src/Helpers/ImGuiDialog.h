#include <string>
#include <functional>

namespace ImGui {
    bool NewFileDialig(const char* label, std::function<void(const std::string&)> callback);
    bool BeginDialogText(const char* label);
    bool EndDialogText(std::function<void(bool)> saveCallBack);
}