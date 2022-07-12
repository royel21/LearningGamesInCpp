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
    Plutus::AppGeo app;
    app.Run();
    return 0;
}