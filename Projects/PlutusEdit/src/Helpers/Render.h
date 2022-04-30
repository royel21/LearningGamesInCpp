#pragma once

#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/FrameBuffer.h>
#include <Graphics/SpriteBatch2D.h>

#include <vector>

namespace Plutus
{
    class Scene;
    struct Config;
    class DebugRender;
    class TransformComponent;
    struct PhysicBodyComponent;

    class Render
    {
    public:
        Config* mConfig = nullptr;
        Camera2D mCamera;
        FrameBuffer mFrameBuffer;
        FrameBuffer mFramePicker;
        std::vector<Renderable> mRenderables;
        int mTotalTemp = 0;
        bool isLoaded = false;

        ~Render();

        //Initialize the shader, camera viewport size and debug render shader
        void init(Config* config);
        void reload(Config* config);

        void draw();

        void resizeBuffers(const Vec2f& size);

        void setScene(Scene* scene) { mScene = scene; }

    private:
        void prepare();
        void drawFixtures(PhysicBodyComponent* pbody, TransformComponent* trans);
        void drawPhysicBodies();

    private:
        Scene* mScene = nullptr;
        Shader mShader;
        SpriteBatch2D mSpriteBatch;
        DebugRender* mDebugRender = nullptr;
    };
} // namespace Plutus