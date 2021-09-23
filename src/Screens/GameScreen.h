#pragma once
#include "Core/IGameScreen.h"
#include "ECS/Scene.h"

#include <Graphics/Shader.h>
#include "Graphics/Camera2D.h"
#include <Graphics/SpriteBatch2D.h>

#include <Systems/SystemManager.h>

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
    Plutus::Ref<Plutus::Scene> mScene;
    Plutus::Camera2D mWorldCamera;
    Plutus::Shader mShader;
    Plutus::SpriteBatch2D mRender;
    Plutus::SystemManager mSystemManager;
};