#include "EditorScreen.h"
#include <Graphics/GLSL.h>
#include <Core/Engine.h>

#include <Physics/Shapes.h>

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
    mAssets->mTextures.addTexture("player2", "assets/textures/zombie.png");
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

    mDebug = Plutus::DebugRender::get();
    mDebug->init(&mCamera);

    mRender.setShader(&mShader);
    mRender.setCamera(&mCamera);
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

    if (mInput->onKeyDown("MouseLeft"))
    {
        // int h = mEngine->getHeight();
        // auto pos = mInput->getMouseCoords();
        // pos.y = h - pos.y;
        // auto mpos = mCamera.convertScreenToWold(pos);
        auto mPos = mInput->getMouseCoords();
        mCamera.setPosition(mPos);
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

    std::vector<Plutus::Renderable> renderables;
    mAssets->mFonts.createRenderable(renderables, "arial", "hello world_Royel _=+/&^%$#ñÑá - Arial", 32.0f, 175.0f, 1.0f, { 255, 255, 255, 255 });
    mAssets->mFonts.createRenderable(renderables, "OpenSansBold", "hello world_Royel _=+/&^%$#ñÑá - OpenSans", 32.0f, 120.0f, 1.0f, { 255, 255, 255, 255 });
    mAssets->mFonts.createRenderable(renderables, "Zoika", "hello world_Royel _=+/&^%$# Zoika", 32.0f, 65.0f, 1.0f, { 255, 255, 255, 255 });

    mRender.submit(renderables);

    mRender.begin();
    mRender.draw(Plutus::BATCH_TEXT);
    mRender.end();

    std::vector<Plutus::Renderable> renderables2;
    renderables2.push_back({ mAssets->mTextures.getTexture("player2")->texId, { 50, h - 100, 64, 64 }, { 0, 0, 1, 1 }, { 255, 255, 255, 255 }, 0, false, false, 0, 0, false });
    renderables2.push_back({ mAssets->mTextures.getTexture("bats")->texId, { 180, h - 280, 96, 96 }, { 0, 0, 1, 1 }, { 255, 255, 255, 255 }, 0, false, false, 99, 0, false });
    renderables2.push_back({ Plutus::AssetManager::get()->mTextures.getTexture("bats")->texId, { 180, 220, 64, 64 }, { 0, 0, 1, 1 }, { 255, 255, 255, 255 }, 0, false, false, 30, 0, false });
    mRender.submit(renderables2);

    mRender.begin();

    mFB.bind();
    mRender.draw(Plutus::BATCH_PICKING);
    mFB.unBind();

    mRender.draw();
    mRender.end();

    // mRender.submit(mFB.getTextureId(), { 300, 300, 300, 300 }, { 0,1,1,0 }, { 255,255,255,255 }, 0, false, false, 32);
    // mRender.begin(&mShader, &mCamera);
    // mRender.draw();
    // mRender.end();
    auto box = Plutus::Box2d(10, h - 160.0f, 150, 150);
    mDebug->drawBox(box);
    mDebug->drawCircle({ 85.0f, h - 85.0f }, 70.0f);
    mDebug->end();
    mDebug->render(2);
    mDebug->setColor({ 255,255,255,25 });
    mDebug->drawGrid();
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