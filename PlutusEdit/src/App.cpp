#include "App.h"

#include "Config.h"

#include <Serialize/Serialize.h>

#include <cstdio>

namespace Plutus
{
    App::App() {

    }

    App::App(const char* name, int width, int height) : Core(name, Config.winWidth, Config.winHeight) {

    };

    App::~App() {

    }

    void App::Setup() {

    }

    void App::Update(float dt) {
    }

    void App::Draw() {

    }
    void App::Exit() {

    }
} // namespace Plutus
