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

        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }


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
        mRenderables.push_back({ nullptr, { 200,200, 100, 50 }, uv, color });
        color.g = 255;
        color.b = 0;
        mRenderables.push_back({ nullptr, { 640,350, 100, 200 }, uv, color });

        mLights.push_back({ nullptr, { -400, -400, 400, 400 }, { -1.0f, -1.0f, 1.0f, 1.0f }, { 255, 255,255,255 } });
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

        if (mShader.enable()) {
            auto mpos = Input::get()->getMouseCoords();
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

            mBatch.draw(&mShader);

            mShader.disable();

            Graphic::enableBlend(true);

            mLightShader.enable();
            mLightShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
            mLightShader.setUniform2f("u_mouse", Input::get()->getMouseCoords());
            mLightShader.setUniform2f("u_resolution", mCamera.getViewPortSize());

            auto& light = mLights[0];
            light.trans.x = mpos.x - 200;
            light.trans.y = mpos.y - 200;

            for (auto& r : mLights) {
                mBatch.addSprite(&r);
            }

            mBatch.draw(&mLightShader);

            mLightShader.disable();

            glBindVertexArray(0);
            mIbo.unbind();
            Graphic::enableBlend();
        }
        // Logger::info("time %0.4f", Time::seconds());
        // Math::Log(Input::get()->getMouseCoords());
    }
}