#include "App.h"

#ifdef __EMSCRIPTEN__
#include <functional>
#include <emscripten.h>
#include <Log/Logger.h>
#include <Time/Timer.h>
std::function<void()> loop2;
void main_loop2() { loop2(); }
#endif


#ifdef __EMSCRIPTEN__
#endif


int main(int argc, char** argv)
{

#ifdef __EMSCRIPTEN__
    u64 end = 0, start = 0;
    loop2 = [&]() {
        end = Plutus::Time::micros();
        auto elapse = end - start;
        start = end;
        Logger::info("time : %llu", elapse);
    };
    emscripten_set_main_loop(main_loop2, 0, true);
#else
    Plutus::AppGeo app;
    app.Run();
#endif


    return 0;
}