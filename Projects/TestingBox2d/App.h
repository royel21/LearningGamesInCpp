#pragma once
#include <Core.h>
#include <Physics/Shapes.h>
#include <box2d/box2d.h>
#include <vector>

#include <Graphics/Shader.h>
#include <Graphics/SpriteBatch2D.h>

#include <memory>

namespace Plutus
{
    class DebugRender;
    struct Texture2;

    class App : public Core
    {
    public:
        App() = default;
        App(const char* name, int width, int height);
        ~App();
        void Setup() override;
        void Update(float dt) override;
        void Draw() override;
        void Exit() override;

        void createBox(float x, float y, float w, float h, int type = 0, float friction = 1);
        void createLine(float x1, float y1, float x2, float y2);
    private:
        float timeStep = 1 / 60.0f;
        int32_t velIter = 6;
        int32_t posIter = 2;
        DebugRender* mDebug;
        Texture2* texture;
        Shader mShader;
        SpriteBatch2D mBatch;
        std::unique_ptr<b2World> mWorld;
        std::vector<Shape*> mShapes;
    };
} // namespace Plutus