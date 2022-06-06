#pragma once

#include <Core/Project.h>
#include <Core/Window.h>
#include <Graphics/Shader.h>

#include <vector>

#include <glm/glm.hpp>
#include <Input/Input.h>

#include <Time/Limiter.h>
#include <Graphics/Camera2D.h>
#include <Systems/SystemManager.h>
#include <Utils/CameraControl.h>

namespace Plutus
{
    class DebugRender;

    struct Point {
        float x;
        float y;
        float index;
        int texIndex;
        Point(float _x, float _y, float i, int tIndex) : x(_x), y(_y), index(i), texIndex(tIndex) {}
    };

    class AppGeo
    {
    public:
        void run();
    private:
        Window mWindow;
        Project mProject;
        Input* mInput;
        SystemManager mSysManager;
        Camera2D mCamera;
        Limiter mLimiter;
        CameraControl mCamControl;
        DebugRender* mDebug;
        // Shader mShader;
        // uint32_t mVertexArray;
        // uint32_t mBufferArray;

        // int tileWidth;
        // int tileHeight;
        float speed = 1;

        // glm::vec2 pos = { 0, 0 };
        // glm::mat4 projection;

        // std::vector<Point> points;

        void init();
        void update();
        void draw();
    };
} // namespace Plutus