#pragma once
#include "System.h"
#include "Graphics/Shader.h"
#include "Graphics/SpriteBatch2D.h"

namespace Plutus
{
    class Camera2D;

    class RenderSystem : public ISystem
    {
        RenderSystem(Scene *scene);
        void update(float dt){};
        void draw(Camera2D *camera);
        void destroy();

    private:
        Shader mShader;
        SpriteBatch2D mRenderer;
    };
} // namespace Plutus
