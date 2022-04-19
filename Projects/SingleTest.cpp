#include <stdio.h>
#include <string>
#include <cstring>

#include <iostream>
#include <sstream>

template <typename... TArgs>
std::string joinPath(TArgs ...args) {
    std::stringstream ss;
    int i = 0;
    auto size = sizeof...(args) - 1;
    ([&](const auto& arg) {
        if (i == 0) {
            ss << arg;
        }
        else {
            auto str = ss.str();
            if (str[str.length() - 1] == '/') {
                ss << arg;
            }
            else {
                ss << "/" << arg;
            }
        }
        i++;
        }(args), ...);
    return ss.str();
}

int main(int argc, char** argv)
{

    std::cout << joinPath("./", "assets", "sound");
    return 0;
}