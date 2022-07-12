#include <iostream>
#include <Math/Vectors.h>
#include <Math/PMath.h>
#include <Time/Timer.h>
#include <thread>
#include <Log/Logger.h>


using namespace Plutus;

using namespace std::chrono_literals;

int main(int argc, char const* argv[])
{

    u64 end = 0, start = 0;

    for (uint32_t i = 0; i < 100 * 10000; i++)
    {
        end = Plutus::Time::micros();
        auto elapse = end - start;
        start = end;
        std::this_thread::sleep_for(30ms);
        Logger::info("time : %llu", elapse / 1000);
    }


    return 0;
}
