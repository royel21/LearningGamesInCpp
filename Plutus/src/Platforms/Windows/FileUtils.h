#pragma once
#include <string>


#define SAVE_FILE 0
#define OPEN_FILE 1

namespace Plutus
{
    bool windowDialog(int mode, std::string& path, const std::string& title = "Open");
    bool copyFile(const std::string& from, const std::string& to);
} // namespace Plutus
