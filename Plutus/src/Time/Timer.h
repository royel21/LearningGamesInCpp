#ifndef __TIMER_H__
#define __TIMER_H__

using u64 = unsigned long long;

namespace Plutus
{
    struct Timer
    {
        static void init();
        static u64 micros();
        static u64 millis();
    };
} // namespace Plutus

#endif // __TIMER_H__