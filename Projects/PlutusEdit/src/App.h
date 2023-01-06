#pragma once
#include <Core/Core.h>
#include <Systems/SystemManager.h>

#include "Config.h"
#include "Panels/WindowManager.h"

namespace Plutus
{
    class App : public Core
    {
    private:
        bool isInitialize = false;
        Config mConfig;
        WindowManager mWinManager;

    public:
        App() = default;
        App(const char* name, int width, int height);
        ~App();

        void Init() override;
        void Update(float dt) override;
        void Dropfile(const char* file) override;
        void Draw() override;
        void Exit() override;
    };
} // namespace Plutus