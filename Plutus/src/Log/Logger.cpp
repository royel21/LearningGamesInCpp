#include "Logger.h"

#include <ctime>
#include <iomanip>
#include <sstream>

#pragma warning(disable : 4996)

std::stringstream ss;

std::string Logger::getFmt(const char* color, const char* level, const std::string& fmt) {
    std::time_t t = std::time(nullptr);
    ss.str("");
    ss << "\033[0;" << color << " " << std::put_time(std::localtime(&t), "%T") << level << " " << fmt << " \n\033[0m";
    return ss.str();
}
