#include "EditorScreen.h"
#include <Graphics/GLSL.h>
#include <Core/Engine.h>

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

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
    mAssets = Plutus::AssetManager::get();
    mAssets->mTextures.addTexture("player", "assets/textures/zombie.png");
    mAssets->mTextures.addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);

    mAssets->mFonts.addFont("arial", "./assets/fonts/arial.ttf", 32);
    mAssets->mFonts.addFont("OpenSansBold", "./assets/fonts/OpenSans-Bold.ttf", 32);
    mAssets->mFonts.addFont("Zoika", "./assets/fonts/Zoika.ttf", 32);

    mRender.init();
    mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);

    const int w = mEngine->getWidth();
    const int h = mEngine->getHeight();

    mCamera.init(w, h);

    mFB.init(w, h, true);

    mDebug = Plutus::DebugRender::geInstances();
    mDebug->init(&mCamera);
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

    if (mInput->onKeyPressed("MouseLeft"))
    {
        int h = mEngine->getHeight();
        auto pos = mInput->getMouseCoords();
        pos.y = h - pos.y;
        std::printf("Id: %i\n", mFB.getEntId(pos));
    }

    if (mInput->onKeyDown("Ctrl"))
    {
        auto scroll = mInput->getMouseWheel();
        if (scroll != 0)
        {
            auto newVal = mCamera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
            mCamera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
        }
    }
    mCamera.update();
}

void EditorScreen::draw()
{
    setBackgoundColor(0, 0, 0, 1);
    int h = mEngine->getHeight();

    auto text = mAssets->mFonts.renderText("arial", "hello world_Royel _=+/&^%$#ñÑá - Arial", 32.0f, h - 175.0f, 1.0f, { 255, 255, 255, 255 });
    mRender.submit(text);

    text = mAssets->mFonts.renderText("OpenSansBold", "hello world_Royel _=+/&^%$#ñÑá - OpenSans", 32.0f, h - 120.0f, 1.0f, { 255, 255, 255, 255 });
    mRender.submit(text);

    text = mAssets->mFonts.renderText("Zoika", "hello world_Royel _=+/&^%$# Zoika", 32.0f, h - 65.0f, 1.0f, { 255, 255, 255, 255 });
    mRender.submit(text);

    mRender.begin(&mShader, &mCamera, true);
    mRender.draw();
    mRender.end();

    mRender.submit(mAssets->mTextures.getTexture("player")->texId, { 50, 100, 64, 64 }, { 0, 1, 1, 0 }, { 255, 255, 255, 255 }, 0, false, false, 25);
    mRender.submit(mAssets->mTextures.getTexture("bats")->texId, { 50, 200, 96, 96 }, { 0, 1, 1, 0 }, { 255, 255, 255, 255 }, 0, false, false, 99);
    mRender.submit(Plutus::AssetManager::get()->mTextures.getTexture("bats")->texId, { 50, 100, 64, 64 }, { 0, 1, 1, 0 }, { 255, 255, 255, 255 }, 0, false, false, 25);

    mRender.begin(&mShader, &mCamera);

    mFB.bind();
    mRender.draw(true);
    mFB.unBind();

    mRender.draw();
    mRender.end();

    // mRender.submit(mFB.getTextureId(), { 300, 300, 300, 300 }, { 0,1,1,0 }, { 255,255,255,255 }, 0, false, false, 32);
    // mRender.begin(&mShader, &mCamera);
    // mRender.draw();
    // mRender.end();

    mDebug->drawBox({ 10, 10, 150, 150 }, { 255, 255, 255, 255 }, 0.0f);
    mDebug->drawCircle({ 85.0f, 85.0f }, { 255, 255, 255, 255 }, 70.0f);
    mDebug->end();
    mDebug->render(2);
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