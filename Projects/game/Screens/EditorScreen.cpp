#include "EditorScreen.h"

#include <Input/Input.h>
#include <Physics/Shapes.h>


#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

EditorScreen::EditorScreen()
{
}

EditorScreen::~EditorScreen()
{
}

void EditorScreen::Init()
{
    // mAssets = Plutus::AssetManager::get();
    // mAssets->addAsset<Plutus::Texture>("player2", "assets/textures/zombie.png");
    // mAssets->addAsset<Plutus::Texture>("bats", "assets/textures/monster/bat.png", 3, 32, 32);

    // mAssets->addAsset<Plutus::Font>("arial", "./assets/fonts/arial.ttf", 32);
    // mAssets->addAsset<Plutus::Font>("OpenSansBold", "./assets/fonts/OpenSans-Bold.ttf", 32);
    // mAssets->addAsset<Plutus::Font>("Zoika", "./assets/fonts/Zoika.ttf", 32);

    // mRender.init();
    // mShader.init(GLSL::vertexShader, GLSL::fragShader);

    // mCamera.init(w, h);

    // mFB.init(w, h, true);

    // mDebug = Plutus::DebugRender::get();
    // mDebug->init(&mCamera);

    // mRender.setShader(&mShader);
    // mRender.setCamera(&mCamera);
}

void EditorScreen::Enter() {

}


void EditorScreen::Update(float dt)
{

    if (Plutus::Input.onKeyPressed("PageUp"))
    {
        mCore->setNextScreen("Game");
    }

    if (Plutus::Input.onKeyDown("MouseLeft"))
    {
        // int h = mEngine->getHeight();
        // auto pos = Plutus::Input.getMouseCoords();
        // pos.y = h - pos.y;
        // auto mpos = mCamera.convertScreenToWold(pos);
        auto mPos = Plutus::Input.getMouseCoords();
        mCamera.setPosition(mPos);
    }

    if (Plutus::Input.onKeyDown("Ctrl"))
    {
        auto scroll = Plutus::Input.getMouseWheel();
        if (scroll != 0)
        {
            auto newVal = mCamera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
            mCamera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
        }
    }
}

void EditorScreen::Draw()
{
    // int h = mEngine->getHeight();

    // std::vector<Plutus::Renderable> renderables;

    // mRender.submit("arial", "hello world_Royel _=+/&^%$#ñÑá - Arial", 32.0f, 175.0f);
    // mRender.submit("OpenSansBold", "hello world_Royel _=+/&^%$#ñÑá - OpenSans", 32.0f, 120.0f);
    // mRender.submit("Zoika", "hello world_Royel _=+/&^%$# Zoika", 32.0f, 65.0f);

    // mRender.finish(Plutus::BATCH_TEXT);

    // std::vector<Plutus::Renderable> renderables2;
    // renderables2.push_back({ mAssets->getAsset<Plutus::Texture>("player2")->mTexId, { 50, h - 100, 64, 64 }, { 0, 0, 1, 1 }, { 255, 255, 255, 255 }, 0, false, false, 0, 0, false });
    // renderables2.push_back({ mAssets->getAsset<Plutus::Texture>("bats")->mTexId, { 180, h - 280, 96, 96 }, { 0, 0, 1, 1 }, { 255, 255, 255, 255 }, 0, false, false, 99, 0, false });
    // renderables2.push_back({ mAssets->getAsset<Plutus::Texture>("bats")->mTexId, { 180, 220, 64, 64 }, { 0, 0, 1, 1 }, { 255, 255, 255, 255 }, 0, false, false, 30, 0, false });
    // mRender.submit(renderables2);

    // mRender.begin();

    // mFB.bind();
    // setBackgoundColor(1, 0.5f, 0.4f);
    // mRender.draw(Plutus::BATCH_PICKING);
    // mFB.unBind();

    // mRender.draw();
    // mRender.end();

    // // mRender.submit(mFB.getTextureId(), { 300, 300, 300, 300 }, { 0,1,1,0 }, { 255,255,255,255 }, 0, false, false, 32);
    // // mRender.begin(&mShader, &mCamera);
    // // mRender.draw();
    // // mRender.end();
    // auto box = Plutus::Box2d(10, h - 160.0f, 150, 150);
    // mDebug->submitBox(box);
    // mDebug->submitCircle({ 85.0f, h - 85.0f }, 70.0f);
    // mDebug->end();
    // mDebug->render(2);
    // mDebug->setColor({ 255,255,255,25 });
    // mDebug->drawGrid();
}
