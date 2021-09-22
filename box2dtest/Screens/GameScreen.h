#pragma once
#include "Core/IGameScreen.h"
#include <Graphics/DebugRenderer.h>
#include <Graphics/Camera2D.h>

class GameScreen : public Plutus::IGameScreen
{
public:
    GameScreen();

    ~GameScreen();

    int getNextScreenIndex() const override;
    int getPrevScreentIndex() const override;

    // called at beginning and end of the application
    void build() override;
    void destroy() override;
    // Called when a screen enter and exit focus
    void onEntry() override;
    void onExit() override;
    // Called in the main game loop
    void update(float deltaTime) override;
    void draw() override;
    void onScreenResize(int w, int h) override;

private:
    Plutus::DebugRender* mDebug;
    Plutus::Camera2D mCamera;
    int mWidth = 50;
    int height = 20;
};