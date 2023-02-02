#pragma once
#include <iostream>
#include <vector>

#include <stb_sprintf.h>

#define Print std::printf


#define BUFFSIZE 512

struct Logger {
    template<class... Args>
    static void error(const char* fmt, Args... args) {
#ifdef DEBUG
        auto format = getFmt("91m", "[ERROR]", fmt);
        std::vector<char> zc(BUFFSIZE);
        int err = stbsp_snprintf(zc.data(), zc.size(), format.c_str(), args...);
        std::cout << zc.data();
#endif
    }

    template<class... Args>
    static void warn(const char* fmt, Args... args) {
#ifdef DEBUG
        auto format = getFmt("96m", "[WARN]", fmt);
        std::vector<char> zc(BUFFSIZE);
        int err = stbsp_snprintf(zc.data(), zc.size(), format.c_str(), args...);
        std::cout << zc.data();
#endif
    }

    template<class... Args>
    static void info(const char* fmt, Args... args) {
#ifdef DEBUG
        auto format = getFmt("33m", "[INFO]", fmt);
        std::vector<char> zc(BUFFSIZE);
        int err = stbsp_snprintf(zc.data(), zc.size(), format.c_str(), args...);
        std::cout << zc.data();
#endif
    }
private:
    static std::string getFmt(const char* color, const char* level, const std::string& fmt);
};