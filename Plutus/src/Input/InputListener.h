#pragma once
#include <string>

namespace Plutus
{
    class InputListener
    {
    public:
        virtual void onKeyDown(const std::string& key) {};
        virtual void onKeyUp(const std::string& key) {};
        virtual void onMouseMove(float x, float y) {};
    };
} // namespace Plutus