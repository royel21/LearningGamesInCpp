#pragma once
#include <string>

namespace Plutus
{
    struct TagComponent
    {
        std::string Name;
        bool Visible = true;
        TagComponent(const std::string name) : Name(name) {}
    };
} // namespace Plutus