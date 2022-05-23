#pragma once
#include <Math/Vectors.h>
#include <Input/InputListener.h>

namespace Plutus
{
    class Camera2D;

    class CameraControl : public InputListener
    {
        Camera2D* mCamera;
        Vec2f mCamPos;
        Vec2f mMousePos;
        float mScaleSpeed = 0.1f;
    public:
        CameraControl();
        void setCamera(Camera2D* camera) { mCamera = camera; }
        void setScale(float scale) { mScaleSpeed = scale; }

        void onKeyDown(const std::string& key) override;
        void onMouseMove(float x, float y) override;
        void onWheel(int scroll) override;
    };
} // namespace Plutus