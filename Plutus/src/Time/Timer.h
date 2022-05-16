#pragma once
#include <string>

using u64 = unsigned long long;

namespace Plutus
{
    struct Time
    {
        static void init();
        static u64 micros();
        static u64 millis();
        static float seconds();
        static void Log(const std::string& id);
        static void LogEnd(const std::string& id);
    };

    class Timer {
        u64 millis = 0;
        u64 micros = 0;
        u64 once = 0;
    public:
        Timer() = default;
        bool IntervalMillis(u64 elapse);
        bool IntervalMicro(u64 elapse);
        bool timeOut(u64 elapse);

        inline void resetTTimeOut() { once = 0; }
    };
} // namespace Plutus
