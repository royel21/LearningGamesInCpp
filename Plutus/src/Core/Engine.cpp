#include "Engine.h"
#include "IGameScreen.h"

#include <Assets/AssetManager.h>
#include <Time/Timer.h>
#include <chrono>
#include <cstdio>

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

	Engine::Engine(const char *name, int w, int h) : mWinName(name), mScreenWidth(w), mScreenHeight(h)
	{
		mWindow.init(name, w, h);
		mInput = Input::getInstance();
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
		std::printf("running \n");

		Clock::time_point startTime, endTime;
#ifdef __EMSCRIPTEN__
		uint64_t end = 0;
		loop = [&]
		{
			auto start = Timer::millis();
			mLastElapsed = static_cast<double>(start - end) / 1000.0f;
			end = start;
#else
		while (!mWindow.isFinish())
		{
			auto startTime = Clock::now();
#endif
			setBackgoundColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			update(static_cast<float>(mLastElapsed));
			draw();
			mInput->update();
			mWindow.update();
#ifndef __EMSCRIPTEN__
			Duration drawTime = Clock::now() - startTime;
			if (limitFPS && drawTime.count() < mSpecFPS)
			{
				Sleep(static_cast<DWORD>((mSpecFPS - drawTime.count()) * 1000.0f));
			}
			Duration currentFrame = Clock::now() - startTime;
			mLastElapsed = currentFrame.count();
#endif

			mFrameTime += mLastElapsed;
			mnFrameTime++;

			if (mFrameTime > 1.000)
			{
				mFps = 1.0f / static_cast<float>(mFrameTime / mnFrameTime);
				mFrameTime = 0.0f;
				mnFrameTime = 0;
			}
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
				mCurrentScreen->onExit();
				mCurrentScreen = mScreenList->moveNext();
				if (mCurrentScreen != nullptr)
				{
					mCurrentScreen->setRunning();
					mCurrentScreen->onEntry();
				}
				break;
			}
			case ScreenState::CHANGE_PREV:
			{
				mCurrentScreen->onExit();
				mCurrentScreen = mScreenList->movePrev();
				if (mCurrentScreen != nullptr)
				{
					mCurrentScreen->setRunning();
					mCurrentScreen->onEntry();
				}
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
