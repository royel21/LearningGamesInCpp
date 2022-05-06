#ifndef __TIMER_H__
#define __TIMER_H__

#include <string>

using u64 = unsigned long long;

namespace Plutus
{
    struct Timer
    {
        static void init();
        static u64 micros();
        static u64 millis();
        static void Log(const std::string& id);
        static void LogEnd(const std::string& id);
    };
} // namespace Plutus

#endif // __TIMER_H__