#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>

#include <ECS/Components/TransformComponent.h>

namespace Plutus
{
	void Camera2D::init(int screenWidth, int screenHeight, bool isViewPortWindowSize)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		if (isViewPortWindowSize) {
			mWindowWidth = screenWidth; mWindowHeight = screenHeight;
		}

		mOrtho = glm::ortho(0.0f, floorf((float)mScreenWidth / mScale), 0.0f, floorf((float)mScreenHeight / mScale), 0.0f, 100.0f);

		update();
	}

	void Camera2D::update()
	{
		if (mEntity) {
			mCamPos = mEntity.getPosition() + mOffset;
			if (mHasBounds) {
				if (mCamPos.x < mBounds.x) mCamPos.x = mBounds.x;
				if (mCamPos.x > mBounds.z) mCamPos.x = mBounds.z;
				if (mCamPos.y < mBounds.y) mCamPos.y = mBounds.y;
				if (mCamPos.y > mBounds.w) mCamPos.y = mBounds.w;
			}
		}

		auto view = glm::lookAt(glm::vec3{ mCamPos.x, mCamPos.y, 10.0f }, { mCamPos.x, mCamPos.y, -1 }, { 0,1,0 });
		mCameraMatrix = mOrtho * view;
	}

	Vec4f Camera2D::getViewPortDim()
	{
		return  Vec4f{ mCamPos.x, mCamPos.y, mScreenWidth / mScale, mScreenHeight / mScale };
	}

	Vec2f Camera2D::convertScreenToWold(Vec2f coords, bool invertY)
	{
		auto coordsTrans = Vec2f{ coords.x / mWindowWidth, coords.y / mWindowHeight };

		return mCamPos + Vec2f{ mScreenWidth / mScale * coordsTrans.x, mScreenHeight / mScale * coordsTrans.y };
	}

	bool Camera2D::isBoxInView(const Vec4f& box)
	{
		float width = box.z / mScale * 2;
		float height = box.w / mScale * 2;
		auto vp = getViewPortDim();

		return (
			box.x < vp.x + vp.z + width &&
			box.y < vp.y + vp.w + height &&
			box.x + box.z > vp.x - width &&
			box.y + box.w > vp.y - height
			);
	}
} // namespace Plutus