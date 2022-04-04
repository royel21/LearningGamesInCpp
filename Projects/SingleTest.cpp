#pragma once
#include <iostream>
#include <Log/Logger.h>
#include <Time/Timer.h>

int main(int argc, char** argv) {
    Logger::error("Some Error %s %i", "testing", 2);
    Logger::warn("Some Warn %s %i", "testing", 1);
    Logger::info("Some info %s %i %i", "testing", 3, 'S');

    return 0;
}