#define SDL_MAIN_HANDLED
#include "App.h"

int main(int argc, char const* argv[])
{
    App* app = new App("Physics Test", 1280, 768);
    app->Run();

    delete app;
    return 0;
}
