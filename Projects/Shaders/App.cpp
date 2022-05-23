#include "App.h"

#include <Math/Vectors.h>

#include <Graphics/Graphic.h>

#include <Input/Input.h>
#include <Time/Timer.h>
#include <Log/Logger.h>
#include <filesystem>

#include <Math/LogVec.h>
#include <Assets/Assets.h>
#include <Graphics/Renderables.h>

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

    // bufferVertices[mVertexCount++] = { mbottomLeft.x, mbottomLeft.y, uv.x, uv.w, c, id };
    // bufferVertices[mVertexCount++] = { mtopLeft.x, mtopLeft.y, uv.x, uv.y, c, id };
    // bufferVertices[mVertexCount++] = { mtopRight.x, mtopRight.y, uv.z, uv.y, c, id };
    // bufferVertices[mVertexCount++] = { mBottomRight.x, mBottomRight.y, uv.z, uv.w, c, id };

    void App::Init()
    {
        mWindow.setAlwaysOnTOp(true);
        mShader.init(ver, frag);
        // Vec4f uv(0, 0, 1, 1);

        // mVertices.push_back({ 0, 0, uv.x, uv.w });//Bottom
        // mVertices.push_back({ 0, 400, uv.x, uv.y });//top left
        // mVertices.push_back({ 400, 400, uv.z, uv.y });//top right
        // mVertices.push_back({ 400, 0, uv.z, uv.w });//bottom right

        // mVAO = Graphic::createVertexArray();
        // mVBO = Graphic::createBufferArray();

        // auto vsize = sizeof(VertexPoint);
        // Graphic::setFAttribute(0, 2, vsize, NULL);
        // Graphic::setFAttribute(1, 2, vsize, (void*)offsetof(VertexPoint, u));

        // glBufferData(GL_ARRAY_BUFFER, mVertices.size() * vsize, mVertices.data(), GL_STATIC_DRAW);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // mIbo.init(1);

        // glBindVertexArray(0);
        AssetManager::get()->addAsset<Texture>("player.png", "assets/textures/Player1.png");

        mLightShader.init(ver, lightFrag);

        mBatch.setCamera(&mCamera);
        mBatch.init();
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
    }

    void App::Draw()
    {
        Renderable ren1;

        if (mShader.enable()) {
            auto mpos = Input::get()->getMouseCoords();
            auto tex = AssetManager::get()->getAsset<Texture>("player.png");
            mShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
            mShader.setUniform2f("u_resolution", mCamera.getViewPortSize());
            mShader.setUniform2f("u_mouse", mpos);
            mShader.setUniform1f("u_time", Time::seconds());
            mShader.setUniform1i("uSampler", 0);

            Renderable ren1(tex, { 0,0, 400, 400 }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 255, 255,255,255 });
            mBatch.addSprite(&ren1);
            mBatch.draw(&mShader);

            // glBindVertexArray(mVAO);
            // mIbo.bind();

            // if (tex) {
            //     glBindTexture(GL_TEXTURE_2D, tex->mTexId);
            // }
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
            mShader.disable();

            Graphic::enableBlend(true);

            mLightShader.enable();
            mLightShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
            mLightShader.setUniform2f("u_mouse", Input::get()->getMouseCoords());
            mLightShader.setUniform2f("u_resolution", mCamera.getViewPortSize());

            Renderable ren2(nullptr, { mpos.x - 200, mpos.y - 200, 400, 400 }, { -1.0f, -1.0f, 1.0f, 1.0f }, { 255, 255,255,255 });
            mBatch.addSprite(&ren2);
            mBatch.draw(&mLightShader);

            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
            mLightShader.disable();

            glBindVertexArray(0);
            mIbo.unbind();
            Graphic::enableBlend();
        }
        // Logger::info("time %0.4f", Time::seconds());
        // Math::Log(Input::get()->getMouseCoords());
    }
}