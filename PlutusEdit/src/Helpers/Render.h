#pragma once

#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/FrameBuffer.h>
#include <Graphics/SpriteBatch2D.h>

#include <vector>

namespace Plutus
{
    class DebugRender;

    class Render
    {
    public:
        Camera2D mCamera;
        FrameBuffer mFrameBuffer;
        FrameBuffer mFramePicker;
        std::vector<Renderable> mRenderables;
        int mTotalTemp = 0;

        static Render& get();
        //Initialize the shader, camera viewport size and debug render shader
        void Init();
        void draw();

    private:
        Render() = default;
        void prepare();

    private:

        Shader mShader;
        SpriteBatch2D mSpriteBatch;
        DebugRender* mDebugRender = nullptr;
    };
} // namespace Plutus