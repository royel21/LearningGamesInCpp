#ifndef _CAMERA2D_H
#define _CAMERA2D_H

#include <glm/glm.hpp>
#include <Math/Vectors.h>

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
		vec2f mCamPos = { 0, 0 };
		glm::mat4 mCameraMatrix = glm::mat4(0);
		glm::mat4 mOrthoMatrix = glm::mat4(0);

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
		inline void setPosition(const vec2f& pos) { setPosition(pos.x, pos.y); }
		/*
			change the camera to a new with and height
			@param w Width of the camera in pixel
			@param h height of the camera in pixel
		*/
		void setWindowSize(int w, int h) { init(w, h); }

		/*
			change the camera to a new with and height
			@param size glm vec2 size
		*/
		void setWindowSize(const vec2i& size) { init(size.x, size.y); }
		// return the current scale value
		float getScale() { return mScale; }
		// zoom the view port
		void setScale(float newScale)
		{
			mScale = newScale;
			mNeedsMatrixUpdate = true;
		}

		vec4f getViewPortDim();
		// return the view port size in pixels
		vec2f getViewPortSize() { return vec2f(mScreenWidth, mScreenHeight); }
		//Getters
		vec2f getPosition() { return mCamPos; }
		// return the 4x4 camera matrix
		glm::mat4 getCameraMatrix() { return mCameraMatrix; }
		//Convert screen coordination to camera coordination and return it
		vec2f convertScreenToWold(float x, float y, bool invertY = false) { return convertScreenToWold({ x, y }, invertY); };
		//Convert screen coordination to camera coordination and return it
		vec2f convertScreenToWold(vec2f screenCoords, bool invertY = false);

		const vec2f getScaleScreen() { return vec2f(mScreenWidth, mScreenHeight) / mScale; }

		bool isBoxInView(const vec4f box, int offset = 0);
	};
} // namespace Plutus

#endif
