#include "FileCheck.h"

#include <filesystem>
#include <Time/Timer.h>


namespace Plutus
{
    uint64_t getLastModified(const std::string& path) {
        auto ftime = std::filesystem::last_write_time(path);
        return ftime.time_since_epoch() / std::chrono::milliseconds(1);
    }

    bool FileCheck::hasChanged(const std::string& path, uint32_t time)
    {
        auto& file = files[path];

        if (file.mTimer.IntervalMillis(time)) {
            auto lastModified = getLastModified(path);;
            if (lastModified > file.lastModified) {
                file.lastModified = lastModified;
                return true;
            }
        }
        return false;
    }
}