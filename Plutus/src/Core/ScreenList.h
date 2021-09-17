#pragma once
#include <vector>

namespace Plutus
{
    class Engine;
    class IGameScreen;

    class ScreenList
    {
    protected:
        int mCurrenScreenIndex = -1;
        Engine *mEngine = nullptr;
        std::vector<IGameScreen *> mScreens;

    public:
        ScreenList(Engine *enine);
        ~ScreenList();

        inline IGameScreen *moveNext() { return moveScreen(true); }
        inline IGameScreen *movePrev() { return moveScreen(false); }
        IGameScreen *getCurrent();
        IGameScreen *moveScreen(bool next);

        void setScreent(int nextScreen);
        void addScreen(IGameScreen *newScreen);
        void destroy();
    };
} // namespace Plutus