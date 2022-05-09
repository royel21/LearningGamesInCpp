#pragma once
#include <Graphics/Shader.h>

namespace Plutus
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() { mShader.destroy(); }

    protected:
        Shader mShader;
        int mLayer = 0;
    };
} // namespace Plutus