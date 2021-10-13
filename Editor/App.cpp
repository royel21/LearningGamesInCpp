
#include "App.h"

#include "Core/ScreenList.h"

#include "Screens/EditorScreen.h"
#include "Screens/GameScreen.h"

App::App(const char* windowName, int screenWidth, int screenHeight) : Engine(windowName, screenWidth, screenHeight)
{
}

App::~App()
{
}

void App::onInit()
{
}

void App::onResize(int width, int height)
{
    glViewport(0, 0, width, height);
}
void App::onExit()
{
}

void App::addScreen()
{
    mEditorScreen = std::make_unique<EditorScreen>();
    mGameScreen = std::make_unique<GameScreen>();

    mScreenList->addScreen(mEditorScreen.get());
    mScreenList->addScreen(mGameScreen.get());

    mScreenList->setScreent(mEditorScreen->getScreenIndex());
}