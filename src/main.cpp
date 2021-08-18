#define SDL_MAIN_HANDLED
#include "App.h"

int main(int argc, char const *argv[])
{
    App *app = new App("Level Editor", 544, 384);
    // App* app = new App("Level Editor", 1280, 768);
    app->run();

    delete app;
    return 0;
}
