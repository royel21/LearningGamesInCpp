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
    auto size = mEngine->getWindowSize();
    mTextLayer.Init(static_cast<float>(size.x), static_cast<float>(size.y), "./assets/fonts/NotoSansJP-Bold.otf", 32);

    mTextures = Plutus::Textures::get();
    mTextures->addTexture("player", "assets/textures/zombie.png");

    mShader.CreateProgWithShader(vertexShader2, textFrag);
    mCamera.init(static_cast<int>(size.x), static_cast<int>(size.y));
    mRender.init();
    mInput = Plutus::Input::getInstance();
    mTTF.init("./assets/fonts/NotoSansJP-Bold.otf", 46);
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
    setBackgoundColor(1, 1, 1, 1);
    int h = mEngine->getHeight();

    auto text = mTTF.getRenderable(32.0f, h - 42.0f, "hello world_Royel");
    mRender.begin(&mShader, &mCamera);
    for (auto r : text) {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }
    mRender.end();
    mTextLayer.drawString("hello world_Royel", 32.0f, 10.0f);

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