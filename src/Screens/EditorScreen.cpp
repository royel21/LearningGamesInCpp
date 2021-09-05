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
    mFontManager = Plutus::FontManager::get();
    auto size = mEngine->getWindowSize();

    mTextures = Plutus::Textures::get();
    mTextures->addTexture("player", "assets/textures/zombie.png");

    mShader.CreateProgWithShader(vertexShader2, textFrag);
    mCamera.init(static_cast<int>(size.x), static_cast<int>(size.y));
    mRender.init();
    mInput = Plutus::Input::getInstance();

    mFontManager->addFont("arial", "./assets/fonts/arial.ttf", 32);
    mFontManager->addFont("OpenSansBold", "./assets/fonts/OpenSans-Bold.ttf", 32);
    mFontManager->addFont("Zoika", "./assets/fonts/Zoika.ttf", 32);
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
    setBackgoundColor(0, 0, 0, 1);
    int h = mEngine->getHeight();

    mRender.begin(&mShader, &mCamera);

    mFontManager->setFont("arial");
    auto text = mFontManager->renderText("hello world_Royel - Arial", 32.0f, h - 175.0f, 1.0f, { 255,255,255,255 });
    for (auto r : text) {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }
    mFontManager->setFont("OpenSansBold");
    text = mFontManager->renderText("hello world_Royel - OpenSans", 32.0f, h - 120.0f, 1.0f, { 255,255,255,255 });
    for (auto r : text) {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }
    mFontManager->setFont("Zoika");
    text = mFontManager->renderText("hello world_Royel _=+/&^%$# Zoika", 32.0f, h - 65.0f, 1.0f, { 255,255,255,255 });
    for (auto r : text) {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }

    mRender.end();
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