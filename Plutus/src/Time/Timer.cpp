#include "Timer.h"
#include <chrono>
#include <cstdio>

#include <unordered_map>

#define u64Time std::chrono::duration_cast<TimerClock::CMicros>(TimerClock::Clock::now().time_since_epoch())

namespace Plutus
{
    namespace TimerClock
    {
        using Clock = std::chrono::system_clock;
        using CMicros = std::chrono::microseconds;

        u64 start = 0;
        std::unordered_map<std::string, u64> timers;

    } // namespace TimerClock


    void Time::init()
    {
        TimerClock::start = u64Time.count();
    }

    u64 Time::micros()
    {
        if (!TimerClock::start) Time::init();
        return u64Time.count() - TimerClock::start;
    }

    u64 Time::millis()
    {
        return micros() / 1000;
    }

    float Time::seconds()
    {
        return micros() / 1000.0f / 1000.0f;
    }

    void Time::Log(const std::string& id)
    {
        TimerClock::timers[id] = micros();
    }

    void Time::LogEnd(const std::string& id)
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


    bool Timer::IntervalMillis(u64 elapse)
    {
        if (millis == 0) millis = Time::millis();

        if (Time::millis() - millis > elapse) {
            millis = 0;
            return true;
        }

        return false;
    }

    bool Timer::IntervalMicro(u64 elapse)
    {
        if (micros == 0) micros = Time::millis();

        if (Time::micros() - micros > elapse) {
            micros = 0;
            return true;
        }

        return false;
    }

    bool Timer::timeOut(u64 elapse)
    {
        if (once == -1) return false;

        if (once == 0) once = Time::millis();

        if (Time::millis() - once > elapse) {
            once = -1;
            return true;
        }

        return false;
    }


} // namespace Plutus