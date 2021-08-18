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
}

void EditorScreen::onEntry()
{
    mInput = Plutus::Input::getInstance();
    auto size = mEngine->getWindowSize();
    mInput = Plutus::Input::getInstance();
    mCamera.init(static_cast<int>(size.x), static_cast<int>(size.y));
    mCamera.update();

    mTextLayer.Init(size.x, size.y, "./assets/fonts/Zoika.ttf", 28);

    mEditor = Plutus::EditorUI::getInstance();
    mEditor->Init(size.x, size.y, &mCamera);
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
    mEditor->update(dt);
    mCamera.update();
}

void EditorScreen::draw()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mEditor->bindFB();
    char text[20];
    snprintf(text, 20, "%.1f FPS", mEngine->getFPS());
    mTextLayer.setColor(1.0f, 1.0f, 1.0f);
    mTextLayer.drawString(text, 5.0f, 5.0f, 1.0f, glm::vec4(1, 0, 0, 1));

    mTextLayer.setColor(1.0f, 0.0f, 0.0f, 0.8f);

    mEditor->unBindFB();
    mEditor->DrawUI();
}

void EditorScreen::onScreenResize(int w, int h)
{
}

void EditorScreen::onExit()
{
    mEditor->destroy();
}

void EditorScreen::destroy()
{
}