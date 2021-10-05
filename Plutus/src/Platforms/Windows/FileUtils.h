#pragma once
#include <string>

namespace Plutus
{
    bool windowDialog(int mode, std::string& path);
    bool copyFile(const std::string &from, const std::string &to);
} // namespace Plutus
