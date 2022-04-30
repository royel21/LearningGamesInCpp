#ifndef _CAMERA2D_H
#define _CAMERA2D_H

#include <glm/glm.hpp>
#include <Math/Vectors.h>
#include <ECS/Scene.h>

namespace Plutus
{
	// Camera to transform Opengl normalize "-1.0 - 1.0" space to camera space in pixels
	class Camera2D
	{
	private:
		int mScreenWidth = 0;
		int mScreenHeight = 0;
		int mWindowWidth = 0;
		int mWindowHeight = 0;

		bool mNeedsMatrixUpdate = true;
		float mScale = 1.0f;
		Entity mEntity;
		Vec2f mOffset = { 0, 0 };
		Vec2f mCamPos = { 0, 0 };
		glm::mat4 mOrtho = glm::mat4(0);
		glm::mat4 mCameraMatrix = glm::mat4(0);

	public:
		/*
			initialize the camera
			@param w Width of the camera in pixel
			@param h height of the camera in pixel
		*/
		void init(int vpw, int vph, bool isViewPortWindowSize = true);

		// update the camera if scale or position is changed
		void update();

		//set camera position and shedule a update
		//@param x position x
		//@param y position y
		inline void setPosition(float x, float y)
		{
			mCamPos.x = x;
			mCamPos.y = y;
			update();
		}

		//set camera position and shedule a update
		//@param newPosition glm vec2 position
		inline void setPosition(const Vec2f& pos) { setPosition(pos.x, pos.y); }

		inline void setTarget(Entity entity, const Vec2f& offset = { 0 }) { mEntity = entity; mOffset = offset; }

		inline void setViewPosition(float x, float y) { setViewPosition(Vec2f{ x, y }); };
		void setViewPosition(const Vec2f& v);
		/*
			change the camera to a new with and height
			@param w Width of the camera in pixel
			@param h height of the camera in pixel
		*/
		inline void setWindowSize(int w, int h) { mWindowWidth = w; mWindowHeight = h; }
		/*
			change the camera to a new with and height
			@param size vec2i size
		*/
		inline void setWindowSize(const Vec2i& size) { mWindowWidth = size.x; mWindowHeight = size.y; }
		// return the current scale value
		inline float getScale() { return mScale; }
		// zoom the view port
		inline void setScale(float scale) { mScale = scale > 0 ? scale : 1.0f; init(mScreenWidth, mScreenHeight); }

		Vec4f getViewPortDim();
		// return the view port size in pixels
		Vec2f getViewPortSize() { return Vec2f(mScreenWidth, mScreenHeight); }
		//Getters
		inline Vec2f getPosition() { return mCamPos; }
		// return the 4x4 camera matrix
		inline glm::mat4 getCameraMatrix() { return mCameraMatrix; }
		//Convert screen coordination to camera coordination and return it
		inline Vec2f convertScreenToWold(float x, float y, bool invertY = false) { return convertScreenToWold({ x, y }, invertY); };
		//Convert screen coordination to camera coordination and return it
		Vec2f convertScreenToWold(Vec2f screenCoords, bool invertY = false);

		const Vec2f getScaleScreen() { return Vec2f(mScreenWidth, mScreenHeight) / mScale; }

		bool isBoxInView(const Vec4f& box, int offset = 0);
	};
} // namespace Plutus

#endif
