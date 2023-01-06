#include "App.h"
#include <iostream>
#include <format>

int main(int argc, char** argv)
{
    std::cout << "Testing: " << std::format("{} {}!", "Hello", "world", "something");
    Plutus::AppGeo app;
    app.Run();
    return 0;
}