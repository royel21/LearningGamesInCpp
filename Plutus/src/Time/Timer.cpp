#include "Timer.h"
#include <chrono>
#include <cstdio>

#include <unordered_map>


namespace Plutus
{
    namespace TimerClock
    {
        using Clock = std::chrono::system_clock;
        using Duration = std::chrono::duration<double>;
        using CMicros = std::chrono::microseconds;
        using HIClock = std::chrono::high_resolution_clock;

        u64 start = 0;
        std::unordered_map<std::string, u64> timers;

    } // namespace TimerClock


    void Timer::init()
    {
        TimerClock::start = std::chrono::duration_cast<TimerClock::CMicros>(TimerClock::Clock::now().time_since_epoch()).count();
    }

    u64 Timer::micros()
    {
        if (!TimerClock::start) Timer::init();
        return std::chrono::duration_cast<TimerClock::CMicros>(TimerClock::Clock::now().time_since_epoch()).count() - TimerClock::start;
    }

    u64 Timer::millis()
    {
        return micros() / 1000;
    }

    float Timer::seconds()
    {
        return float(micros() / 1000) / 1000.0f;
    }

    void Timer::Log(const std::string& id)
    {
        TimerClock::timers[id] = micros();
    }

    void Timer::LogEnd(const std::string& id)
    {
        auto found = TimerClock::timers.find(id);
        if (found != TimerClock::timers.end()) {
            std::printf("%s:%llu\n", found->first.c_str(), micros() - found->second);
            TimerClock::timers.erase(id);
        }
        else {
            std::printf("Timer Not Found: %s\n", id.c_str());
        }
    }

} // namespace Plutus