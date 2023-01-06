#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include <Time/Timer.h>

namespace Plutus
{
    struct File {
        Timer mTimer;
        uint64_t lastModified = 0;
        uint32_t elapse = 0;
    };

    class FileCheck
    {
    public:
        FileCheck() {}

        bool hasChanged(const std::string& path, uint32_t time);

    private:
        std::unordered_map<std::string, File> files;
    };
} // namespace Plutus