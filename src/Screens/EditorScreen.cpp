#include "EditorScreen.h"
#include <Graphics/GLSL.h>
#include <Core/Engine.h>
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
    mTextures = Plutus::Textures::get();
    mTextures->addTexture("player", "assets/textures/zombie.png");

    auto size = mEngine->getWindowSize();
    mShader.CreateProgWithShader(vertexShader2, textFrag);
    mCamera.init(static_cast<int>(size.x), static_cast<int>(size.y));
    mRender.init(&mCamera);
    mInput = Plutus::Input::getInstance();
    mTTF.init("c:/windows/fonts/times.ttf", 32);
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
}

void EditorScreen::draw()
{
    setBackgoundColor(1, 0, 0, 1);
    mShader.enable();
    mShader.setUniformMat4("camera", mCamera.getCameraMatrix());

    mTTF.print(32.0f, 32.0f, "hello world - Royel", mRender);

    // mRender.begin();
    // mRender.submit(mTextures->getTexture("player")->texId, { 0,0,64,64 });
    // mRender.end();

    mShader.disable();
}

void EditorScreen::onScreenResize(int w, int h)
{
}

void EditorScreen::onExit()
{
}

void EditorScreen::destroy()
{
}