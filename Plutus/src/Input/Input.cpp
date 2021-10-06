#include "Input.h"
#include <algorithm>

namespace Plutus
{

	Input::Input() : mMouseCoords(0, 0), mKeyMap(0)
	{
	}

	Input* Input::getInstance()
	{
		static Input instance;
		return &instance;
	}

	// save the key state before register new key event
	void Input::update()
	{
		mMouseWheel = 0;
		mMouseMove = false;
		//Loop through keymap and copy it to prevKeyMap.
		for (auto& key : mKeyMap)
		{
			mPrevKeyMap[key.first] = key.second;
		}
	}
	//Register key state
	void Input::keyStateChange(const std::string& keyId, int state)
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
	void Input::setMouseCoords(float x, float y)
	{
		mMouseCoords.x = x;
		mMouseCoords.y = y;
	}

	//return the state of the key on last frame
	bool Input::wasKeyDown(const std::string& keyId)
	{
		auto it = mPrevKeyMap.find(keyId);
		//Check if key was found and return it state else return false
		return it == mPrevKeyMap.end() ? false : it->second;
	}

	// return true if key is being hold down
	bool Input::onKeyDown(std::string keyId)
	{
		auto it = mKeyMap.find(keyId);
		return it == mKeyMap.end() ? false : it->second;
	}

	//return single key press
	bool Input::onKeyPressed(std::string keyId)
	{
		//return true if key was not down last frame
		return onKeyDown(keyId) && !wasKeyDown(keyId);
	}

	void Input::addEventListener(InputListener* listener)
	{
		mEventListeners.push_back(listener);
	}

	void Input::addRemoveListener(InputListener* listener)
	{
		auto end = std::remove_if(mEventListeners.begin(), mEventListeners.end(), [&](auto t1) { return t1 == listener; });
		if (end != mEventListeners.end()) {
			mEventListeners.erase(end, mEventListeners.end());
		}
	}
} // namespace Plutus