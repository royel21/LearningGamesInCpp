#pragma once
// #include <Time/Timer.h>
#include "Assetmanager/AssetManager.h"

int main(int argc, char** argv) {

    auto manager = Plutus::AssetManager2::get();
    // auto start = Plutus::Timer::micros();
    // Logger::error("Some Error %s %i", "testing", 2);

    // printf("time: %llu", Plutus::Timer::micros() - start);

    return 0;
}