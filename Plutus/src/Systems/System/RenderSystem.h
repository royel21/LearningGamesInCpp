#pragma once
#include "System.h"

#include <Graphics/Shader.h>
#include <Graphics/SpriteBatch2D.h>

namespace Plutus
{
    class RenderSystem : public ISystem
    {
    public:
        RenderSystem(Scene* scene, Camera2D* camera);
        ~RenderSystem();
        void update(float dt);
        void destroy();

    private:
        Shader mShader;
        SpriteBatch2D mRenderer;
        std::vector<Renderable> mRenderables;
    };
} // namespace Plutus
