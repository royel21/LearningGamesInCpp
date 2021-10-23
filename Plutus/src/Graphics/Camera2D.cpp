#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Plutus
{
	void Camera2D::init(int screenWidth, int screenHeight)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		//Convert the opengl espace -1.0 - 1.0 to 0 - screenW and height
		mOrthoMatrix = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
		update();
	}

	vec4f Camera2D::getViewPortDim()
	{
		auto half = vec2f(mScreenWidth >> 1, mScreenHeight >> 1) + mCamPos;
		auto halfScale = getScaleScreen() / 2.0f;
		vec4f size = { half.x - halfScale.x, half.y - halfScale.y, half.x + halfScale.x, half.y + halfScale.y };
		return size;
	}

	void Camera2D::update()
	{
		if (mNeedsMatrixUpdate == true)
		{
			mCameraMatrix = glm::translate(mOrthoMatrix, { -mCamPos.x, -mCamPos.y, 0.0f });
			//Camera Scale
			mCameraMatrix = glm::scale(glm::mat4(1.0f), { mScale, mScale, 0.0f }) * mCameraMatrix;
			mNeedsMatrixUpdate = false;
		}
	}

	vec2f Camera2D::convertScreenToWold(vec2f coords, bool invertY)
	{

		if (invertY)
			coords.y = mScreenHeight - coords.y;
		// translate coord to center of the screen
		coords -= vec2f(mScreenWidth >> 1, mScreenHeight >> 1);
		//have to scale the coordinate and the camera current pos
		coords /= mScale;
		auto camScale = mCamPos / mScale;

		//Translate with the camera position
		coords += mCamPos;

		coords += vec2f(mScreenWidth >> 1, mScreenHeight >> 1);

		return coords;
	}

	bool Camera2D::isBoxInView(const vec4f box, int offset)
	{
		auto camSize = getViewPortDim() + vec4f(-offset, -offset, offset, offset);

		if (box.x + box.z > camSize.x && box.y + box.w > camSize.y && box.x < camSize.z && box.y < camSize.w) {
			return true;
		}

		return false;
	}
} // namespace Plutus