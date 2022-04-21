#include <stdio.h>
#include <string>
#include <cstring>

#include <iostream>
#include <sstream>

std::string ToLowerCase(const std::string& str)
{
    auto str2 = str;
    for (auto& s : str2)
    {
        if (s < 91) s = s + 32;
    }
    return str2;
}

int main(int argc, char** argv)
{

    std::cout << ToLowerCase("Textures") << std::endl;
    return 0;
}