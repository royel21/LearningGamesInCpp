#include "SleepImp.h"

#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Plutus
{
        void msSleep(uint32_t millis) {
#ifdef _WIN32
                // if we are on window set the time precision to 1ms for sleep function
                timeBeginPeriod(1);
#endif
                std::this_thread::sleep_for(std::chrono::milliseconds(millis));
#ifdef _WIN32
                timeEndPeriod(1);
#endif
        }
} // namespace Plutus
