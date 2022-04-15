#pragma once

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

#include "../Config.h"

namespace Plutus
{
    struct Config;
    class Render;

    class SceneWindow {

    private:
        Config* mConfig;
        Render* mRender;

    public:
        void init(Config* config, Render* render) {
            mConfig = config;
            mRender = render;
        }
        void draw();
    private:
        void drawEntity(Entity ent);
        void showConfigScene(bool& show, Ref<Scene>& scene);
    };
} // namespace Plutus