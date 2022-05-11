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

		mOrtho = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight, 0.0f, 100.0f);

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

		mCameraMatrix = glm::translate(glm::mat4(1.0f), { mCamPos.x, mCamPos.y, 0.0f });
		mCameraMatrix = mOrtho * glm::scale(mCameraMatrix, { mScale, mScale, 0.0f });
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
		// float width = box.z / mScale * 2;
		// float height = box.w / mScale * 2;
		// auto vp = getViewPortDim();

		// return(
		// 	box.x < vp.x + vp.z + width &&
		// 	box.y < vp.y + vp.w + height &&
		// 	box.x + box.z > vp.x - width &&
		// 	box.y + box.w > vp.y - height
		// 	);
		glm::vec2 pos(box.x, box.y);
		glm::vec2 dim(box.z, box.w);

		glm::vec2 scaleDim = glm::vec2(mScreenWidth, mScreenHeight) / mScale;

		const float MIN_DISTANCE_X = dim.x / 2.0f + scaleDim.x / 2.0f;
		const float MIN_DISTANCE_Y = dim.y / 2.0f + scaleDim.y / 2.0f;

		glm::vec2 centerPos = pos + dim / 2.0f;

		glm::vec2 distVec = centerPos - pos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		return xDepth > 0 && yDepth > 0;
	}
} // namespace Plutus