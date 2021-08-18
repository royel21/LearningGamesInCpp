#pragma once
#include "System.h"
#include "Graphics/Shader.h"
#include "Graphics/SpriteBatch2D.h"

namespace Plutus
{
    class Camera2D;

    class RenderSystem : public System
    {
        RenderSystem();
        void init(Camera2D *camera);
        void draw(entt::registry *_register);
        void destroy();

    private:
        Shader mShader;
        SpriteBatch2D mRenderer;
        Camera2D *mCamera = nullptr;
    };
} // namespace Plutus
