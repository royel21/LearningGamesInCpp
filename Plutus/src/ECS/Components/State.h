#pragma once
#include <string>

namespace Plutus
{
    class MachineState
    {
        std::string getState() { return mState; };
        virtual void setState(const std::string& t);
        virtual void nextState();
        virtual void prevState();
        virtual void update(float dt);

    private:
        std::string mState;
        std::string mPrevState;
    };
} // namespace Plutus