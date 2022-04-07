#include "Logger.h"

#include <sstream>

std::stringstream ss;

std::string Logger::getFmt(const char* level, const char* fmt) {
    ss.str("");
    ss << "\033[0;" << level << " " << fmt << "\033[0m\n";
    return ss.str();
}

int Logger::valid(const char* str, int c) {
    int count = 0;
    while (*str++)
        if (*str == '%') count++;

    if (count > c) {
        Print("\033[0;91mParams Error Args: %i Params: %i\n\033[0m", c, count);
        return false;
    }
    return true;
}