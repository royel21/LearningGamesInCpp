#pragma once
#include <Core/type.h>
#include <ECS/Scene.h>
#include <string>

#include <Physics/Vectors.h>

namespace Plutus
{
    struct ViewPortState {
        int wdith = 1280;
        int height = 768;
        vec2f mouseCoords;
        bool isHover;
    };

    struct AppContext
    {
        Entity mEnt;
        Ref<Scene> mScene;
        std::string currentScene;
        ViewPortState mVpState;

        static AppContext& get();

        void NewScene();
        void Load(const char* filepath);
        void Save(const char* filepath, Ref<Scene>& mScene);

    private:
        AppContext();
    };

    extern AppContext& Context;
} // namespace Plutus