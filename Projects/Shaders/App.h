#pragma once
#include <Core/Core.h>

#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>

#include <vector>
#include <cstdint>

#include "FileCheck.h"
#include "SpriteBatch.h"
#include <Graphics/Renderables.h>

namespace Plutus
{
    struct VertexPoint {
        float x, y;
        float u, v;
    };

    class App : public Core
    {
    public:
        App();
        ~App();

        void Init();
        void Update(float);
        void Draw();

    private:
        uint32_t mVAO;
        uint32_t mVBO;

        FileCheck mFileCheck;
        Shader mShader;
        Shader mLightShader;
        IndexBuffer mIbo;
        std::vector<Renderable> mRenderables;
        std::vector<Renderable> mLights;
        SpriteBatch mBatch;
    };
} // namespace Plutus