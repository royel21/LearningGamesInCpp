#include "App.h"
#include <Assets//Assets.h>

#include <Log/Logger.h>
#include <Utils/FileIO.h>

namespace Plutus
{
    void App::Setup() {
        mShader.init();
        mRenderer.init();
        mRenderer.setShader(&mShader);
        mRenderer.setCamera(&mCamera);

        auto manager = AssetManager2::get();

        manager->addAsset<Texture>("Link", "assets/textures/player1.png");
        manager->addAsset<Font>("Zoika", "assets/fonts/Zoika.ttf", 64);
        manager->addAsset<Font>("Arial", "assets/fonts/arial.ttf", 64);
        auto sound = manager->addAsset<Sound>("BG", "assets/sounds/shotgun.wav");
        sound->play();
        // Logger::info(typeid(sound).name());

        Logger::info("shotgun: %i", exists("assets/sounds/shotgun.wav"));
        Logger::info("arial: %i", exists("assets/fonts/arial.ttf"));
        Logger::info("player1: %i", exists("assets/textures/player1.png"));
        Logger::info("Zoika: %i", exists("assets/fonts/Zoika.ttf"));

        setBackgoundColor(1, 0.9f, 0.4f);
    }

    void App::Draw() {
        auto tex = AssetManager2::get()->getAsset<Texture>("Link");

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
