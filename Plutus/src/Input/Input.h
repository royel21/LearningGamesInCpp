#ifndef _INPUT_H
#define _INPUT_H

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <unordered_map>

#include "InputListener.h"

namespace Plutus
{

	class InputEvent
	{
		virtual void handler(uint32_t type, uint32_t key);
	};

	class Input
	{
	private:
		bool mouseMove = false;
		int m_mouseWheel = 0;
		glm::vec2 m_mouseCoords;
		std::unordered_map<std::string, bool> m_keyMap;
		std::unordered_map<std::string, bool> m_prevKeyMap;

	public:
		bool isCtrl = false;
		std::function<void(const char*)> onFileDrop = nullptr;

	public:
		Input();
		static Input* getInstance();

		void update();

		void keyStateChange(const std::string& keyId, int state);

		void setMouseCoords(float x, float y);

		bool onKeyDown(std::string keyId);

		bool onKeyPressed(std::string keyId);

		void setMouseWheel(int dir) { m_mouseWheel = dir; }
		void setMouseMove(bool move) { mouseMove = move; }
		bool getMouseIsMoving() { return mouseMove; }

		int getMouseWheel() { return m_mouseWheel; }

		glm::vec2 getMouseCoords() const { return m_mouseCoords; }

		void addEventListener(InputListener* listener);
		void addRemoveListener(InputListener* listener);

	private:
		bool wasKeyDown(const std::string& keyId);
		std::vector<InputListener*> mEventListeners;
	};
} // namespace Plutus

#endif
