#pragma once

#include <Core/Core.h>
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

    class AppGeo : public Core
    {
    public:
        AppGeo();
        void Init() override;
        void Update(float dt) override;
        void Draw() override;
        void Resize(int width, int height) override;
    private:
        SystemManager mSysManager;
        CameraControl mCamControl;
    };
} // namespace Plutus