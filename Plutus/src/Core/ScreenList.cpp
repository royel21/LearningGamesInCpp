#include "ScreenList.h"
#include "IGameScreen.h"

namespace Plutus
{
    ScreenList::ScreenList(Engine* engine) : mEngine(engine)
    {
    }

    ScreenList::~ScreenList()
    {
        destroy();
    }

    IGameScreen* ScreenList::moveScreen(bool next)
    {
        IGameScreen* currentScreen = getCurrent();
        int nextScreen = next ? currentScreen->getNextScreenIndex() : currentScreen->getPrevScreentIndex();
        currentScreen->onExit();
        if (nextScreen != SCREEN_INDEX_NO_SCREEN)
        {
            mCurrenScreenIndex = nextScreen;
            mScreens[mCurrenScreenIndex]->setRunning();
            mScreens[mCurrenScreenIndex]->onEntry();
        }

        return getCurrent();
    }

    void ScreenList::setScreent(int nextScreen)
    {
        mCurrenScreenIndex = nextScreen;
    }

    void ScreenList::addScreen(IGameScreen* newScreen)
    {
        newScreen->mScreenIndex = static_cast<int>(mScreens.size());
        mScreens.push_back(newScreen);
        newScreen->setEngine(mEngine);
        newScreen->build();
    }

    void ScreenList::destroy()
    {
        mScreens.resize(0);
        mCurrenScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen* ScreenList::getCurrent()
    {
        if (mCurrenScreenIndex == SCREEN_INDEX_NO_SCREEN)
            return nullptr;
        return mScreens[mCurrenScreenIndex];
    }
} // namespace Plutus