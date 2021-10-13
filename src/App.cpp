
#include "App.h"

#include "Core/ScreenList.h"

App::App(const char* windowName, int screenWidth, int screenHeight) : Engine(windowName, screenWidth, screenHeight)
{
}

App::~App()
{
}

void App::onInit()
{
}

void App::onReisze(int width, int height)
{
    glViewport(0, 0, width, height);
    printf("Resize\n");
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