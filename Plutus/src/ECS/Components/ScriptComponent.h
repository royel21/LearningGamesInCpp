#pragma once

#include <string>
#include <sol/sol.hpp>
#include "../Scene.h"

namespace Plutus
{
    class ScriptComponent
    {
    public:
        std::string mScript;
        bool isLoaded = false;

    public:
        ScriptComponent() = default;
        ScriptComponent(const std::string& script);
        ScriptComponent(const ScriptComponent& script);
        ~ScriptComponent() { destroy(); }

        void setScript(const std::string& script);

        void init(sol::state& lua, Entity ent);

        void update(float dt);

        void animationEnd(const std::string id);

        void CollisionStart(uint32_t entId, bool isSensor);
        void CollisionEnd(uint32_t entId, bool isSensor);

        float report(uint32_t id, float frac);

        void destroy();

    private:
        sol::environment mEnv = sol::nil;
    };
} // namespace Plutus class Script