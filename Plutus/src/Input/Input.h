#ifndef _INPUT_H
#define _INPUT_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Plutus
{

	class InputEvent
	{
		virtual void handler(uint32_t type, uint32_t key);
	};

	class Input
	{
	private:
		glm::vec2 m_mouseCoords;
		int m_mouseWheel = 0;
		std::unordered_map<std::string, bool> m_keyMap;
		std::unordered_map<std::string, bool> m_prevKeyMap;
		bool mouseMove = false;

	public:
		Input();
		static Input* getInstance();

		void update();

		void keyStateChange(const std::string& keyId, bool state);

		void setMouseCoords(float x, float y);

		bool onKeyDown(std::string keyId);

		bool onKeyPressed(std::string keyId);

		void setMouseWheel(int dir) { m_mouseWheel = dir; }
		void setMouseMove(bool move) { mouseMove = move; }
		bool getMouseIsMoving() { return mouseMove; }

		int getMouseWheel() { return m_mouseWheel; }

		glm::vec2 getMouseCoords() const { return m_mouseCoords; }

	private:
		bool wasKeyDown(const std::string& keyId);
	};
} // namespace Plutus

#endif
