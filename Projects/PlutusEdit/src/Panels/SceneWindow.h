#pragma once

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

#include <string>

#include "ImWindow.h"
#include <ECS/Scene.h>
#include <Utils/Pointer.h>

namespace Plutus
{
    class SceneWindow : public ImWindow {

    private:
        std::string sceneName;
        std::string newName;

    public:
        void draw();

    private:
        void drawEntity(Entity ent);
        void showConfigScene(bool& show, Ref<Scene>& scene);
    };
} // namespace Plutus