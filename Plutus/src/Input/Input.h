#ifndef _INPUT_H
#define _INPUT_H

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include <Math/Vectors.h>

#include "InputListener.h"

namespace Plutus
{

	class InputEvent
	{
		virtual void handler(uint32_t type, uint32_t key);
	};

	class Input
	{
	public:
		bool isCtrl = false;
		std::function<void(const char*)> onFileDrop = nullptr;
		std::function<void(int, int)> onResize = nullptr;

	public:
		Input();
		// Return a instance of Input Manager
		static Input* get();

		void update();

		void keyStateChange(const std::string& keyId, int state);

		void setMouseCoords(float x, float y);

		bool onKeyDown(std::string keyId);

		bool onKeyPressed(std::string keyId);

		void setMouseWheel(int dir) { mMouseWheel = dir; }
		void setMouseMove(bool move) { mMouseMove = move; }
		bool getMouseIsMoving() { return mMouseMove; }

		int getMouseWheel() { return mMouseWheel; }

		Vec2f getMouseCoords() const { return mMouseCoords; }

		void addEventListener(InputListener* listener);
		void addRemoveListener(InputListener* listener);
	private:
		bool wasKeyDown(const std::string& keyId);

	private:
		bool mMouseMove = false;
		int mMouseWheel = 0;
		Vec2f mMouseCoords;
		std::vector<InputListener*> mEventListeners;
		std::unordered_map<std::string, bool> mKeyMap;
		std::unordered_map<std::string, bool> mPrevKeyMap;
	};
} // namespace Plutus

#endif
