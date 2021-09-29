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

	glm::vec4 Camera2D::getViewPortDim()
	{
		auto half = glm::vec2(mScreenWidth >> 1, mScreenHeight >> 1) + mCamPos;
		auto halfScale = getScaleScreen() / 2.0f;
		glm::vec4 size = { half.x - halfScale.x, half.y - halfScale.y, half.x + halfScale.x, half.y + halfScale.y };
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

	glm::vec2 Camera2D::convertScreenToWold(glm::vec2 coords, bool invertY)
	{
		if (invertY)
			coords.y = mScreenHeight - coords.y;
		// translate coord to center of the screen
		coords -= glm::vec2(mScreenWidth >> 1, mScreenHeight >> 1);
		//have to scale the coordinate and the camera current pos
		coords /= mScale;
		auto camScale = mCamPos / mScale;

		//Translate with the camera position
		coords += mCamPos;

		coords += glm::vec2(mScreenWidth >> 1, mScreenHeight >> 1);

		return coords;
	}

	bool Camera2D::isBoxInView(const glm::vec2 position, const glm::vec2 dim)
	{
		glm::vec2 scaleDim = getScaleScreen();

		const float MIN_DISTANCE_X = dim.x / 2.0f + scaleDim.x / 2.0f;
		const float MIN_DISTANCE_Y = dim.y / 2.0f + scaleDim.y / 2.0f;

		glm::vec2 centerPos = position + dim / 2.0f;

		glm::vec2 distVec = centerPos - mCamPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		return false;
	}
} // namespace Plutus