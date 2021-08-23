#include "RenderSystem.h"
#include "Graphics/Camera2D.h"
#include "Graphics/GLSL.h"
#include "ECS/Scene.h"

namespace Plutus
{
    RenderSystem::RenderSystem(Scene *scene) : ISystem(scene)
    {
        mShader.CreateProgWithShader(vertexShader2, fragShader2);
    }

    void RenderSystem::draw(Camera2D *camera)
    {
    }

    void RenderSystem::destroy()
    {
    }

} // namespace Plutus