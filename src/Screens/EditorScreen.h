#pragma once

#include "Core/IGameScreen.h"
#include "Input/Input.h"
#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/SpriteBatch2D.h>
#include <Graphics/DebugRenderer.h>
#include <Assets/AssetManager.h>

class EditorScreen : public Plutus::IGameScreen
{
public:
    EditorScreen();
    ~EditorScreen();

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
    Plutus::Input* mInput = nullptr;
    Plutus::Shader mShader;
    Plutus::Camera2D mCamera;
    Plutus::SpriteBatch2D mRender;
    Plutus::AssetManager* mAssets;
    Plutus::DebugRender* mDebug;
};