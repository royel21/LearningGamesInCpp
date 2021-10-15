#pragma once
#include <Core.h>

namespace Plutus
{
    class App : public Core
    {
    public:
        App();
        App(const char* name, int width, int height);
        ~App();

        void Setup() override;
        void Update(float dt) override;
        void Draw() override;
        void Exit() override;
    };
} // namespace Plutus