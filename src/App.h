#pragma once
#include "Core/Engine.h"
#include "Screens/EditorScreen.h"
#include "Screens/GameScreen.h"

class App : public Plutus::Engine
{
private:
    std::unique_ptr<EditorScreen> mEditorScreen = nullptr;
    std::unique_ptr<GameScreen> mGameScreen = nullptr;

public:
    App(const char* windowName = "Plutus Game", int screenWidth = 1280, int screenHeight = 768);
    ~App();
    void addScreen() override;
    void onInit() override;
    void onReisze(int width, int height);
    void onExit() override;
};
