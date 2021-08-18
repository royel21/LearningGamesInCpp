#include "Input.h"

namespace Plutus
{

	Input::Input() : m_mouseCoords(0, 0), m_keyMap(0)
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
		m_mouseWheel = 0;
		mouseMove = false;
		//Loop through keymap and copy it to prevKeyMap.
		for (auto& key : m_keyMap)
		{
			m_prevKeyMap[key.first] = key.second;
		}
	}
	//Register key state
	void Input::keyStateChange(const std::string& keyId, bool state)
	{
		m_keyMap[keyId] = state;
	}
	//Set the mouse X,Y position on Screen
	void Input::setMouseCoords(float x, float y)
	{
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	//return the state of the key on last frame
	bool Input::wasKeyDown(const std::string& keyId)
	{
		auto it = m_prevKeyMap.find(keyId);
		//Check if key was found and return it state else return false
		return it != m_prevKeyMap.end() ? it->second : false;
	}

	// return true if key is being hold down
	bool Input::onKeyDown(std::string keyId)
	{
		return m_keyMap[keyId] == true;
	}

	//return single key press
	bool Input::onKeyPressed(std::string keyId)
	{
		//return true if key was not down last frame
		return onKeyDown(keyId) && !wasKeyDown(keyId);
	}
} // namespace Plutus