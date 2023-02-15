#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <algorithm>
#include <Input/Input.h>
#include <Log/Logger.h>

namespace Plutus
{
	void Camera2D::init(int screenWidth, int screenHeight, bool isViewPortWindowSize)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		if (isViewPortWindowSize) {
			mWindowWidth = screenWidth;
			mWindowHeight = screenHeight;
		}

		setScale(mScale);
		update();
	}

	void Camera2D::setScale(float scale) {
		mScale = std::clamp(scale, 0.1f, 20.0f);
		mScaleSize = { mScreenWidth / mScale, mScreenHeight / mScale };
		mOrtho = glm::ortho(0.0f, mScaleSize.x, 0.0f, mScaleSize.y, 0.0f, 100.0f);
	}

	void Camera2D::update()
	{
		if (mEntity && !Input.isCtrl) {
			auto size = mEntity.getRect().getSize();
			mCamPos = mEntity.getPosition() - (mScaleSize / 2.0f - size / 2.0f);
			if (mHasBounds) {
				if (mCamPos.x < mBounds.x) mCamPos.x = mBounds.x;
				if (mCamPos.x > mBounds.z) mCamPos.x = mBounds.z;
				if (mCamPos.y < mBounds.y) mCamPos.y = mBounds.y;
				if (mCamPos.y > mBounds.w) mCamPos.y = mBounds.w;
			}
		}
		Logger::info("x:%0.3f y:%.03f", -mCamPos.x, -mCamPos.y);
		mCameraMatrix = mOrtho * glm::translate(glm::mat4(1.0f), { -roundf(mCamPos.x), -roundf(mCamPos.y), 0.0f });
	}

	Vec4f Camera2D::getViewPortDim()
	{
		return  Vec4f{ mCamPos.x, mCamPos.y, mScaleSize.x, mScaleSize.y };
	}

	Vec2f Camera2D::convertScreenToWold(Vec2f coords, bool invertY)
	{
		auto coordsTrans = Vec2f{ coords.x / mWindowWidth, coords.y / mWindowHeight };
		return mCamPos + Vec2f{ mScaleSize.x* coordsTrans.x, mScaleSize.y* coordsTrans.y };
	}
} // namespace Plutus