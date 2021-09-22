
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

void App::onExit()
{
}

void App::addScreen()
{
    mGameScreen = std::make_unique<GameScreen>();

    mScreenList->addScreen(mGameScreen.get());

    mScreenList->setScreent(mGameScreen->getScreenIndex());
}