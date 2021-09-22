#pragma once
#include <Input/Input.h>

#define SCREEN_INDEX_NO_SCREEN -1

namespace Plutus
{

	class Engine;
	class Input;

	enum class ScreenState
	{
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREV
	};

	class IGameScreen
	{
	protected:
		int mScreenIndex = -1;
		ScreenState mCurrentState = ScreenState::NONE;
		Engine* mEngine = nullptr;
		Input* mInput;

	public:
		friend class ScreenList;

		IGameScreen() = default;

		virtual ~IGameScreen() = default;

		// Return the index of the next or previous screen when changing screen
		virtual int getNextScreenIndex() const = 0;
		virtual int getPrevScreentIndex() const = 0;

		// called at beginning and end of the application
		virtual void build() = 0;

		virtual void destroy() = 0;
		// Called when a screen enter and exit focus
		virtual void onEntry() = 0;

		virtual void onExit() = 0;

		virtual void draw() = 0;
		// Called in the main game loop
		virtual void update(float deltaTime) = 0;

		virtual void onScreenResize(int w, int h) = 0;

		// Return the current screen index
		int getScreenIndex() const { return mScreenIndex; }
		// Set the parent of the screen

		void setRunning() { mCurrentState = ScreenState::RUNNING; }

		ScreenState getState() const { return mCurrentState; }
		// Set the parent of the screen
		void setEngine(Engine* engine)
		{
			mEngine = engine;
			mInput = Input::getInstance();
		}
	};

} // namespace Plutus
