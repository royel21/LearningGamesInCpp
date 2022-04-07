#pragma once
#include <iostream>

#define Print std::printf


struct Logger {
    template<class... Args>
    static void error(const char* fmt, Args... args) {
        if (!valid(fmt, sizeof...(Args))) return;
        Print(getFmt("91m[ERROR]", fmt).c_str(), args...);
    }

    template<class... Args>
    static void warn(const char* fmt, Args... args) {
        if (!valid(fmt, sizeof...(Args))) return;
        Print(getFmt("96m[WARN]", fmt).c_str(), args...);
    }

    template<class... Args>
    static void info(const char* fmt, Args... args) {
        if (!valid(fmt, sizeof...(Args))) return;
        Print(getFmt("93m[INFO]", fmt).c_str(), args...);
    }

    private:
        static int valid(const char* str, int c);

        static std::string getFmt(const char* level, const char* fmt);
};