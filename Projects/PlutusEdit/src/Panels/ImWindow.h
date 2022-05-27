#pragma once

namespace Plutus
{
    struct Config;

    class ImWindow
    {
    protected:
        Config* mConfig;
    public:
        virtual void init(Config* config) { mConfig = config; }

        virtual void draw() {}
        virtual void update(float dt) {}
    };
} // namespace Plutus