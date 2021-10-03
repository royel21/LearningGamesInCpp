#include "EditorScreen.h"

#include <cstdlib>

#include "Assets/AssetManager.h"
#include "Core/Engine.h"
#include "Utils/Utils.h"

EditorScreen::EditorScreen()
{
}

EditorScreen::~EditorScreen()
{
    mEditor->destroy();
}

int EditorScreen::getNextScreenIndex() const
{
    return 1;
}

int EditorScreen::getPrevScreentIndex() const
{
    return 1;
}

void EditorScreen::build()
{
    mInput = Plutus::Input::getInstance();

    auto size = static_cast<glm::ivec2>(mEngine->getWindowSize());
    mCamera.init(size.x, size.y);
    mCamera.update();

    mEditor = Plutus::EditorUI::getInstance();
    mEditor->Init(&mCamera);
}

void EditorScreen::onEntry()
{
}

void EditorScreen::update(float dt)
{

    if (mInput->onKeyPressed("PageUp"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_PREV;
    }
    if (mInput->onKeyPressed("PageDown"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_NEXT;
    }
    mCamera.update();
    mEditor->update(dt);
}

void EditorScreen::draw()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mEditor->drawUI();
}

void EditorScreen::onScreenResize(int w, int h)
{
}

void EditorScreen::onExit()
{
}

void EditorScreen::destroy()
{
    mEditor->destroy();
}