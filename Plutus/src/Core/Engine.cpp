#include "Engine.h"
#include "IGameScreen.h"

#include <Assets/AssetManager.h>
#include <Time/Timer.h>
#include <chrono>
#include <cstdio>
#include <GLFW/glfw3.h>

#include <memory>

#ifdef __EMSCRIPTEN__
#include <functional>
#include <emscripten.h>
#endif

using Clock = std::chrono::system_clock;
using Duration = std::chrono::duration<float>;

namespace Plutus
{

#ifdef __EMSCRIPTEN__
	std::function<void()> loop;
	void main_loop() { loop(); }
#endif

	Engine::Engine(const char* name, int w, int h) : mWinName(name), mScreenWidth(w), mScreenHeight(h)
	{
		mWindow.init(name, w, h);
		mInput = Input::get();
		mScreenList = std::make_unique<ScreenList>(this);
	}

	Engine::~Engine()
	{
	}

	void Engine::onEvent(int code, int type)
	{
	}

	void Engine::exitGame()
	{
		mWindow.close();
	}

	void Engine::run()
	{
		if (!init())
			return;

#ifdef __EMSCRIPTEN__
		loop = [&]
		{
#else
		while (mWindow.isFinish())
		{
#endif
			float dt = mLimiter.start();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			update(dt);
			draw();
			mInput->update();
			mWindow.update();
			mLimiter.end();
		};
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(main_loop, 0, true);
#endif

		std::printf("finnish \n");
		mCurrentScreen->onExit();
		if (mScreenList)
		{
			mScreenList->destroy();
			mScreenList.reset();
		}

		onExit();
		AssetManager::get()->clearData();
	}

	bool Engine::update(float dt)
	{
		if (mCurrentScreen)
		{
			switch (mCurrentScreen->getState())
			{
			case ScreenState::RUNNING:
			{
				mCurrentScreen->update(dt);
				break;
			}
			case ScreenState::CHANGE_NEXT:
			{
				mCurrentScreen = mScreenList->moveNext();
				break;
			}
			case ScreenState::CHANGE_PREV:
			{
				mCurrentScreen = mScreenList->movePrev();
				break;
			}
			case ScreenState::EXIT_APPLICATION:
			{
				mWindow.close();
				return true;
				break;
			}
			default:
				break;
			}
		}
		else
		{
			mWindow.close();
			return true;
		}
		return false;
	}

	void Engine::draw()
	{
		if (mCurrentScreen && mCurrentScreen->getState() == ScreenState::RUNNING)
		{
			mCurrentScreen->draw();
		}
	}

	bool Engine::init()
	{
		Timer::init();
		addScreen();
		mCurrentScreen = mScreenList->getCurrent();
		mCurrentScreen->onEntry();
		mCurrentScreen->setRunning();
		glViewport(0, 0, mScreenWidth, mScreenHeight);
		return true;
	}

} // namespace Plutus
