#include "Input.h"
#include <algorithm>

namespace Plutus
{

	InputManager::InputManager() : mMouseCoords(0, 0), mKeyMap(0)
	{
	}

	// save the key state before register new key event
	void InputManager::update()
	{
		mMouseWheel = 0;
		mMouseMove = false;
		//Loop through keymap and copy it to prevKeyMap.
		for (auto& [key, value] : mKeyMap)
		{
			mPrevKeyMap[key] = value;
		}
	}
	//Register key state
	void InputManager::keyStateChange(const std::string& keyId, int state)
	{
		mKeyMap[keyId] = state > 0;

		if (state < 2) {

			if (keyId == "Ctrl") isCtrl = state == 1;

			for (auto listener : mEventListeners) {
				state ? listener->onKeyDown(keyId) : listener->onKeyUp(keyId);
			}
		}
	}
	//Set the mouse X,Y position on Screen
	void InputManager::setMouseCoords(float x, float y)
	{
		mMouseCoords.x = x;
		mMouseCoords.y = y;
		for (auto listener : mEventListeners) {
			listener->onMouseMove(x, y);
		}
	}

	//return the state of the key on last frame
	bool InputManager::wasKeyDown(const std::string& keyId)
	{
		auto it = mPrevKeyMap.find(keyId);
		//Check if key was found and return it state else return false
		return it == mPrevKeyMap.end() ? false : it->second;
	}

	// return true if key is being hold down
	bool InputManager::onKeyDown(std::string keyId)
	{
		auto it = mKeyMap.find(keyId);
		return it == mKeyMap.end() ? false : it->second;
	}

	//return single key press
	bool InputManager::onKeyPressed(std::string keyId)
	{
		//return true if key was not down last frame
		return onKeyDown(keyId) && !wasKeyDown(keyId);
	}

	void InputManager::setMouseWheel(int dir)
	{
		mMouseWheel = dir;
		for (auto listener : mEventListeners) {
			listener->onWheel(dir);
		}
	}

	void InputManager::addEventListener(InputListener* listener)
	{
		mEventListeners.push_back(listener);
	}

	void InputManager::addRemoveListener(InputListener* listener)
	{
		auto found = std::remove_if(mEventListeners.begin(), mEventListeners.end(), [&](auto t1) { return t1 == listener; });
		if (found != mEventListeners.end()) {
			mEventListeners.erase(found, mEventListeners.end());
		}
	}

	InputManager Input = InputManager();
} // namespace Plutus