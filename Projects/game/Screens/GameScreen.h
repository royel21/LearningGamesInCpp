#pragma once

#include <Core/IScreen.h>
#include <Systems/SystemManager.h>


class GameScreen : public Plutus::IScreen
{
public:
    GameScreen();

    ~GameScreen();
    // called at beginning and end of the application
    void Init() override;
    void Enter() override;
    void Draw();
    // Called when a screen enter and exit focus
    void Update(float dt) override;
    void Exit() override;

private:
    Plutus::SystemManager mSystemManager;
};