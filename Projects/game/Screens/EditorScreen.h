#pragma once


#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/FrameBuffer.h>
#include <Graphics/SpriteBatch2D.h>
#include <Graphics/DebugRenderer.h>

#include <Core/IScreen.h>
#include <Assets/Assets.h>


class EditorScreen : public Plutus::IScreen
{
public:
    EditorScreen();
    ~EditorScreen();

    // called at beginning and end of the application
    void Init() override;
    // Called when a screen enter and exit focus
    void Enter() override;
    // Called in the main game loop
    void Update(float dt) override;
    void Draw() override;

private:
    Plutus::FrameBuffer mFB;
    Plutus::DebugRender* mDebug;
    Plutus::AssetManager* mAssets;

    Plutus::Shader mShader;
    Plutus::Camera2D mCamera;
    Plutus::SpriteBatch2D mRender;
};