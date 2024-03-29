
#include "App.h"

#include "Screens/GameScreen.h"
#include "Screens/EditorScreen.h"

App::App(const char* name, int width, int height)
{
    mProject.load("ZombiesGame/ZombiesGame.json");
    mName = name;
}

App::~App()
{
}

void App::Init()
{
    addScreen("Game", new GameScreen(), true);
    addScreen("Editor", new EditorScreen());
}


void App::Exit()
{
}
