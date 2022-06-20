#include "App.h"

#include <Math/Vectors.h>

#include <Graphics/Graphic.h>

#include <Input/Input.h>
#include <Time/Timer.h>
#include <Log/Logger.h>
#include <filesystem>

#include <Math/LogVec.h>
#include <Assets/Assets.h>


namespace Plutus
{
    const char* ver = "Projects/Shaders/ver.vert";
    const char* frag = "Projects/Shaders/frag.frag";
    const char* lightFrag = "Projects/Shaders/lightfrag.frag";

    App::App()
    {
        mWidth = 1280;
        mHeight = 768;
        mName = "Shader App";
    }

    App::~App()
    {
        mShader.destroy();
        mIbo.destroy();
        mLightShader.destroy();
        mSBatch.destroy();
        Graphic::destroy(&mVAO, &mVBO);
    }

    float size = 200;

    void App::Init()
    {
        mShader.init(ver, frag);

        auto tex = AssetManager::get()->addAsset<Texture>("player.png", "assets/textures/Player1.png");

        mLightShader.init(ver, lightFrag);

        mBatch.setCamera(&mCamera);
        mBatch.init();
        Vec4f uv = { 0.0f, 0.0f, 1.0f, 1.0f };
        ColorRGBA8 color(255, 255, 255, 255);

        mRenderables.push_back({ tex, { 0,0, 128, 128 }, uv,color });
        color.g = 0;
        mRenderables.push_back({ nullptr, { 200,200, 100, 100 }, uv, color });
        color.g = 255;
        color.b = 0;
        mRenderables.push_back({ nullptr, { 640,350, 100, 200 }, uv, color });
        mLights.push_back({ nullptr, { -size, -size, size, size }, { -1.0f, -1.0f, 1.0f, 1.0f }, { 255, 255,255,255 } });

        mSBatch.init(&mCamera);

    }

    void App::Update(float)
    {
        if (mFileCheck.hasChanged(ver, 60)) {
            mShader.init(ver, frag);
            mLightShader.init(ver, lightFrag);
        }

        if (mFileCheck.hasChanged(frag, 60)) {
            mShader.init(ver, frag);
        }

        if (mFileCheck.hasChanged(lightFrag, 60)) {
            mLightShader.init(ver, lightFrag);
        }

        if (Input::get()->onKeyPressed("Z")) {
            isAlwaysOnTop = !isAlwaysOnTop;
            mWindow.setAlwaysOnTOp(isAlwaysOnTop);
        }

        if (Input::get()->onKeyPressed("X")) {
            blink = true;
        }
    }

    Timer timer;

    void App::Draw()
    {
        Renderable ren1;
        auto mpos = Input::get()->getMouseCoords();

        Graphic::setBackgoundColor(0, 0);
        if (mShader.enable()) {
            Graphic::disableBlend();
            mShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
            mShader.setUniform2f("u_resolution", mCamera.getViewPortSize());
            mShader.setUniform2f("u_mouse", mpos);
            mShader.setUniform1f("u_time", Time::seconds());
            mShader.setUniform1i("uSampler", 0);

            if (blink && timer.IntervalMillis(200)) {
                blink = false;
            }

            mShader.setUniform1b("blink", blink);

            for (auto& r : mRenderables) {
                mBatch.addSprite(&r);
            }

            Graphic::enableBlend();
            mBatch.draw(&mShader);

            mShader.disable();
        }

        Graphic::enableBlend(true);

        mLightShader.enable();
        mLightShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
        mLightShader.setUniform2f("u_mouse", Input::get()->getMouseCoords());


        int w = Input::get()->getMouseWheel();

        if (w != 0) {
            if (w > 0) {
                size += 4.0f;
            }
            else {
                size -= 4.0f;
            }
        }


        auto& light = mLights[0];
        light.trans.x = mpos.x - size * .5f;
        light.trans.y = mpos.y - size * .5f;
        light.trans.z = size;
        // light.trans.w = size;

        // if (Input::get()->onKeyDown("Z")) {
        //     light.trans.w += 4;
        // }
        if (Input::get()->onKeyDown("X")) {
            light.trans.w -= 4;
        }

        if (Input::get()->onKeyDown("Scape")) {
            close();
        }

        // mLightShader.setUniform2f("u_resolution", { light.trans.z, light.trans.w });
        mLightShader.setUniform2f("u_resolution", mCamera.getViewPortSize());


        for (auto& r : mLights) {
            mBatch.addSprite(&r);
        }

        mBatch.draw(&mLightShader);

        mLightShader.disable();

        Graphic::enableBlend();

        // mSBatch.addRect({ 97, 447, 206, 206 }, 30, { 255, 0, 0, 100 });
        // mSBatch.addRect({ 100, 450, 200,200 }, 25, { 255,255,255,100 });

        // mSBatch.addCircle({ 100, 300 }, 206, { 0,0,0 });
        // mSBatch.addCircle({ 100, 300 }, 200);
        // mSBatch.draw();
    }
    // Logger::info("time %0.4f", Time::seconds());
    // Math::Log(Input::get()->getMouseCoords());
}