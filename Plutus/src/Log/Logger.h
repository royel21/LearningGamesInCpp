#pragma once
#include <iostream>

#define Print std::printf


struct Logger {
    template<class... Args>
    static void error(const char* fmt, Args... args) {
        auto format = getFmt("91m", "[ERROR]", fmt);
        std::vector<char> zc(255);
        int err = std::snprintf(zc.data(), zc.size(), format.c_str(), args...);
        std::cout << zc.data();
    }

    template<class... Args>
    static void warn(const char* fmt, Args... args) {
        auto format = getFmt("96m", "[WARN]", fmt);
        std::vector<char> zc(255);
        int err = std::snprintf(zc.data(), zc.size(), format.c_str(), args...);
        std::cout << zc.data();
    }

    template<class... Args>
    static void info(const char* fmt, Args... args) {
        auto format = getFmt("93m", "[INFO]", fmt);
        std::vector<char> zc(255);
        int err = std::snprintf(zc.data(), zc.size(), format.c_str(), args...);
        std::cout << zc.data();
    }

private:
    static std::string getFmt(const char* color, const char* level, const std::string& fmt);
};