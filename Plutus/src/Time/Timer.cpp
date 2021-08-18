#include "Timer.h"
#include <chrono>

using Clock = std::chrono::system_clock;
using Duration = std::chrono::duration<double>;
using CMicros = std::chrono::microseconds;
using HIClock = std::chrono::high_resolution_clock;

namespace Plutus
{
    u64 start = 0;

    void Timer::init()
    {
        start = std::chrono::duration_cast<CMicros>(Clock::now().time_since_epoch()).count();
    }

    u64 Timer::micros()
    {
        if (!start) Timer::init();
        return std::chrono::duration_cast<CMicros>(Clock::now().time_since_epoch()).count() - start;
    }

    u64 Timer::millis()
    {
        return micros() / 1000;
    }

} // namespace Plutus