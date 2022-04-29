#pragma once

namespace Plutus
{
    struct Config;
    class Render;

    class BottomWindow
    {
    private:
        Config* mConfig;
        Render* mRender;
    public:
        void init(Config* config, Render* render) {
            mConfig = config;
            mRender = render;
        }
        void draw();
    };
} // namespace Plutus