#ifndef _CAMERA2D_H
#define _CAMERA2D_H

#include "glm/glm.hpp"

namespace Plutus
{
	// Camera to transform Opengl normalize "-1.0 - 1.0" space to camera space in pixels
	class Camera2D
	{
	private:
		int mScreenWidth = 0;
		int mScreenHeight = 0;
		bool mNeedsMatrixUpdate = true;
		float mScale = 1.0f;
		glm::vec2 mCamPos;
		glm::mat4 mCameraMatrix;
		glm::mat4 mOrthoMatrix;

	public:
		/*
			initialize the camera
			@param w Width of the camera in pixel
			@param h height of the camera in pixel
		*/
		void init(int w, int h);
		// update the camera if scale or position is changed
		void update();
		//set camera position and shedule a update
		//@param x position x
		//@param y position y
		void setPosition(float x, float y)
		{
			mCamPos.x = x;
			mCamPos.y = y;
			mNeedsMatrixUpdate = true;
		}
		//set camera position and shedule a update
		//@param newPosition glm vec2 position
		void setPosition(const glm::vec2& newPosition)
		{
			mCamPos = newPosition;
			mNeedsMatrixUpdate = true;
		}
		/*
			change the camera to a new with and height
			@param w Width of the camera in pixel
			@param h height of the camera in pixel
		*/
		void setWindowSize(float w, float h) { init(static_cast<int>(w), static_cast<int>(h)); }

		/*
			change the camera to a new with and height
			@param size glm vec2 size
		*/
		void setWindowSize(const glm::vec2& size) { init(static_cast<int>(size.x), static_cast<int>(size.y)); }
		// return the current scale value
		float getScale() { return mScale; }
		// zoom the view port
		void setScale(float newScale)
		{
			mScale = newScale;
			mNeedsMatrixUpdate = true;
		}
		// return the view port size in pixels
		glm::vec2 getViewPortSize() { return glm::vec2(mScreenWidth, mScreenHeight); }
		//Getters
		glm::vec2 getPosition() { return mCamPos; }
		// return the 4x4 camera matrix
		glm::mat4 getCameraMatrix() { return mCameraMatrix; }
		//Convert screen coordination to camera coordination and return it
		glm::vec2 convertScreenToWold(float x, float y) { return convertScreenToWold({ x, y }); };
		//Convert screen coordination to camera coordination and return it
		glm::vec2 convertScreenToWold(glm::vec2 screenCoords);

		const glm::vec2 getScaleScreen() { return glm::vec2(mScreenWidth, mScreenHeight) / mScale; }

		bool isBoxInView(const glm::vec2 position, const glm::vec2 dim);
	};
} // namespace Plutus

#endif
