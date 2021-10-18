
#include "GameScreen.h"
#include <Graphics/GLheaders.h>
#include <Physics/Shapes.h>

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
}

int GameScreen::getNextScreenIndex() const
{
    return 0;
}

int GameScreen::getPrevScreentIndex() const
{
    return 0;
}

void GameScreen::build()
{
    mDebug = Plutus::DebugRender::get();
    mCamera.init(70, 60);
    mDebug->init(&mCamera);
}

void GameScreen::onEntry()
{

}

void GameScreen::update(float dt)
{

}

void GameScreen::draw()
{
    setBackgoundColor(1, 0, 0, 1);

    mDebug->drawBox(Plutus::Box2d(1, 1, 2, 2));
    mDebug->drawCircle({ 25.0f, 10.0f }, 4.0f);
    mDebug->end();
    mDebug->render(1);
}

void GameScreen::onScreenResize(int w, int h)
{
}

void GameScreen::onExit()
{

}

void GameScreen::destroy()
{
}
