#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"

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
		update();
	}

	vec4f Camera2D::getViewPortDim()
	{
		// return  vec4f{ mCamPos, mScreenWidth / mScale, mScreenHeight / mScale };
		auto half = vec2f(mScreenWidth >> 1, mScreenHeight >> 1) + mCamPos;
		auto halfScale = getScaleScreen() / 2.0f;
		vec4f size = { half.x - halfScale.x, half.y - halfScale.y, half.x + halfScale.x, half.y + halfScale.y };
		return size;
	}

	void Camera2D::update()
	{
		vec2f pos = mCamPos;
		if (mEntity.isValid()) {
			auto trans = mEntity.getComponent<TransformComponent>();
			if (trans) {
				auto half = vec2f(mScreenWidth, mScreenHeight);
				pos = trans->getPosition() + mOffset;

			}
		}
		// mViewMat4 = glm::translate(glm::mat4(1), { -mCamPos.x, -mCamPos.y, 0.0f });
		// mProjectionMat4 = glm::ortho(0.0f, (float)mScreenWidth / mScale, 0.0f, (float)mScreenHeight / mScale);
		// mCameraMatrix = mProjectionMat4 * mViewMat4;
		mCameraMatrix = glm::translate(glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight), { -pos.x, -pos.y, 0.0f });
		//Camera Scale
		mCameraMatrix = glm::scale(glm::mat4(1.0f), { mScale, mScale, 0.0f }) * mCameraMatrix;
	}

	void Camera2D::setViewPosition(const vec2f& v) {
		auto half = vec2f(mScreenWidth >> 1, mScreenHeight >> 1) / mScale;
		auto pos = v / mScale;
		setPosition(pos + half);
	}

	vec2f Camera2D::convertScreenToWold(vec2f coords, bool invertY)
	{
		if (invertY)
			coords.y = mScreenHeight - coords.y;
		auto half = vec2f(mScreenWidth >> 1, mScreenHeight >> 1);
		// translate coord to center of the screen
		coords -= half;
		//have to scale the coordinate and the camera current pos
		coords /= mScale;
		auto camScale = mCamPos / mScale;

		//Translate with the camera position
		coords += mCamPos;

		coords += half;

		return coords;

		// auto coordsTrans = vec2f{ coords.x / mWindowWidth, coords.y / mWindowHeight };

		// return mCamPos + vec2f{ mScreenWidth / mScale * coordsTrans.x, mScreenHeight / mScale * coordsTrans.y };

	}

	bool Camera2D::isBoxInView(const vec4f box, int offset)
	{
		auto camSize = getViewPortDim() + vec4f(-offset, -offset, offset, offset);

		if (box.x + box.z > camSize.x && box.y + box.w > camSize.y && box.x < camSize.z && box.y < camSize.w) {
			return true;
		}

		return true;
	}
} // namespace Plutus