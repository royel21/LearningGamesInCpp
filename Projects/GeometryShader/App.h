#pragma once

#include <Core/Project.h>
#include <Core/Window.h>
#include <Graphics/Shader.h>

#include <vector>

#include <glm/glm.hpp>
#include <Input/Input.h>

#include "Camera.h"
#include "MapRender.h"

#include <Graphics/Camera2D.h>
#include <Systems/SystemManager.h>

namespace Plutus
{
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
        Camera mCamera2;
        MapRender mMapRender;
        SystemManager mSysManager;
        Camera2D mCamera;

        // Shader mShader;
        // uint32_t mVertexArray;
        // uint32_t mBufferArray;

        // int tileWidth;
        // int tileHeight;
        float speed = 32;

        // glm::vec2 pos = { 0, 0 };
        // glm::mat4 projection;

        // std::vector<Point> points;

        void init();
        void update();
        void draw();
    };
} // namespace Plutus