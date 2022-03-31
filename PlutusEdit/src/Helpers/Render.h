#pragma once

#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/FrameBuffer.h>
#include <Graphics/SpriteBatch2D.h>

#include <vector>

namespace Plutus
{
    class DebugRender;
    class Scene;

    class Render
    {
    public:
        Camera2D mCamera;
        FrameBuffer mFrameBuffer;
        FrameBuffer mFramePicker;
        std::vector<Renderable> mRenderables;
        int mTotalTemp = 0;

        ~Render();

        static Render& get();
        //Initialize the shader, camera viewport size and debug render shader
        void Init();
        void draw();

        void setScene(Scene* scene) { mScene = scene; }

    private:
        Render() = default;
        void drawPhysicBodies();
        void prepare();

    private:
        Scene* mScene;
        Shader mShader;
        SpriteBatch2D mSpriteBatch;
        DebugRender* mDebugRender = nullptr;
    };
} // namespace Plutus