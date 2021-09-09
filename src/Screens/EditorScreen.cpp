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
    mAssets = Plutus::AssetManager::get();
    auto size = mEngine->getWindowSize();
    mDebug = Plutus::DebugRender::geInstances();
    mDebug->init(&mCamera);


    mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);
    mShaderPicking.CreateProgWithShader(GLSL::vertexShader, GLSL::pickingFragShader);
    mCamera.init(static_cast<int>(size.x), static_cast<int>(size.y));
    mRender.init();
    mInput = Plutus::Input::getInstance();

    mAssets->mTextures.addTexture("player", "assets/textures/zombie.png");
    mAssets->mTextures.addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);

    mAssets->mFonts.addFont("arial", "./assets/fonts/arial.ttf", 32);
    mAssets->mFonts.addFont("OpenSansBold", "./assets/fonts/OpenSans-Bold.ttf", 32);
    mAssets->mFonts.addFont("Zoika", "./assets/fonts/Zoika.ttf", 32);

    mPicker.init(static_cast<int>(size.x), static_cast<int>(size.y));
    mFB.init(static_cast<int>(size.x), static_cast<int>(size.y));
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
    if (mInput->onKeyPressed("MouseLeft")) {
        int h = mEngine->getHeight();
        auto pos = mCamera.convertScreenToWold(mInput->getMouseCoords());
        pos.y = h - pos.y;
        std::printf("Id: %i %.0f %.0f\n", mPicker.getEntId(pos), pos.x, pos.y);
    }
}

void EditorScreen::draw()
{
    setBackgoundColor(0, 0, 0, 1);
    int h = mEngine->getHeight();


    auto text = mAssets->mFonts.renderText("arial", "hello world_Royel - Arial", 32.0f, h - 175.0f, 1.0f, { 255, 255, 255, 255 });
    for (auto r : text)
    {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }

    text = mAssets->mFonts.renderText("OpenSansBold", "hello world_Royel - OpenSans", 32.0f, h - 120.0f, 1.0f, { 255, 255, 255, 255 });
    for (auto r : text)
    {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }

    text = mAssets->mFonts.renderText("Zoika", "hello world_Royel _=+/&^%$# Zoika", 32.0f, h - 65.0f, 1.0f, { 255, 255, 255, 255 });
    for (auto r : text)
    {
        mRender.submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY);
    }

    mRender.begin(&mShader, &mCamera, true);
    mRender.draw();
    mRender.end();

    mRender.submit(mAssets->mTextures.getTexture("player")->texId, { 50, 100, 64, 64 }, { 0,1,1,0 }, { 255,255,255,255 }, 0, false, false, 25);
    mRender.submit(mAssets->mTextures.getTexture("bats")->texId, { 50, 200, 96, 96 }, { 0,1,1,0 }, { 255,255,255,255 }, 0, false, false, 99);

    mRender.begin(&mShaderPicking, &mCamera);

    mPicker.bind();
    mRender.draw(true);
    mPicker.unBind();

    mRender.begin(&mShader, &mCamera);
    mRender.draw();
    mRender.end();

    mRender.submit(mPicker.getTextureId(), { 300, 300, 300, 300 }, { 0,1,1,0 }, { 255,255,255,255 }, 0, false, false, 32);
    mRender.begin(&mShader, &mCamera);
    mRender.draw();
    mRender.end();

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