#include "App.h"
#include <Assets//Assets.h>

#include <Log/Logger.h>
#include <Utils/FileIO.h>
// #include "Logger.h"
#include <Time/Timer.h>

#include <unordered_map>

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
        // sound->play();
        // Logger::info(typeid(sound).name());

        setBackgoundColor(1, 0.9f, 0.4f);

        auto start = Timer::micros();
        // Plutus::Logger::error("Welcome to spdlog!");
        // Plutus::Logger::info("Some error message with arg: {}", 1);
        // Plutus::Logger::warn("Easy padding in numbers like {:08d}", 12);
        // Plutus::Logger::error("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
        // Plutus::Logger::info("Support for floats {:03.2f}", 1.23456);
        // Plutus::Logger::warn("Positional args are {1} {0}..", "too", "supported");
        // Plutus::Logger::error("{:<30}", "left aligned");
        // Plutus::Logger::info("time: {}", Timer::micros() - start);
        start = Timer::micros();
        Logger::error("Welcome to spdlog!");
        Logger::info("Some error message with arg: {}", 1);
        Logger::warn("Easy padding in numbers like {:08d}", 12);
        Logger::error("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
        Logger::info("Support for floats {:03.2f}", 1.23456);
        Logger::warn("Positional args are {1} {0}..", "too", "supported");
        Logger::error("{:<30}", "left aligned");
        Logger::info("time: %llu", Timer::micros() - start);

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
