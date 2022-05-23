#include "CameraControl.h"
#include <Input/Input.h>

#include <Graphics/Camera2D.h>
#include <Log/Logger.h>

namespace Plutus
{
    CameraControl::CameraControl()
    {
        Input::get()->addEventListener(this);
    }

    void CameraControl::onKeyDown(const std::string& key)
    {
        if (Input::get()->isCtrl && key == "MouseLeft") {
            mMousePos = Input::get()->getMouseCoords();
            mCamPos = mCamera->getPosition();
        }
    }

    void CameraControl::onMouseMove(float x, float y) {
        if (Input::get()->isCtrl && Input::get()->onKeyDown("MouseLeft")) {
            auto pos = Input::get()->getMouseCoords();
            Vec2f result = mCamPos - ((pos - mMousePos) / mCamera->getScale());
            mCamera->setPosition(result);
        }
    }

    void CameraControl::onWheel(int scroll)
    {
        if (Input::get()->isCtrl) {
            auto scale = mCamera->getScale();
            auto pos = Input::get()->getMouseCoords();

            auto scalePos = pos / scale;
            auto newVal = scale + (scroll > 0 ? 0.1f : -0.1f);
            mCamera->setScale(newVal);

            auto newPos = pos / mCamera->getScale();

            auto offset = newPos - scalePos;
            mCamera->setPosition(mCamera->getPosition() - offset);
        }
    }
}