#include "App.h"
#include <Assets/temp/Assets.h>

#include <Log/Logger.h>

namespace Plutus
{
    void App::Setup() {
        mShader.init();
        mRenderer.init();
        mRenderer.setShader(&mShader);
        mRenderer.setCamera(&mCamera);

        auto manager = AssetManager2::get();

        manager->addAsset<Texture2>("Link", "assets/textures/player1.png");
        manager->addAsset<Font>("Zoika", "assets/fonts/Zoika.ttf", 64);
        manager->addAsset<Font>("Arial", "assets/fonts/arial.ttf", 64);
        manager->addAsset<Sound>("BG", "assets/sounds/XYZ2.ogg");

        // Logger::info(typeid(sound).name());

        setBackgoundColor(1, 0.9f, 0.4f);
    }

    void App::Draw() {
        auto tex = AssetManager2::get()->getAsset<Texture2>("Link");

        mRenderer.submit(tex->mTexId, { mWidth / 2, mHeight / 2, 64, 64 }, tex->getUV(0));
        mRenderer.submit(tex->mTexId, { 32, 32, 128, 128 }, tex->getUV(0));
        mRenderer.finish();

        mRenderer.submit("Zoika", "Testing Font", 0, mHeight - 64.0f);
        mRenderer.submit("Arial", "Testing Font", 0, mHeight - 128.0f);
        mRenderer.finish(BATCH_TEXT);
    }

    void App::Exit() {
        AssetManager2::get()->destroy();
        mShader.destroy();
    }
} // namespace Plutus
