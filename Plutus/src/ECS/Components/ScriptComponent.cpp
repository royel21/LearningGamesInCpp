#include "ScriptComponent.h"
#include <Assets/Assets.h>

namespace Plutus
{
    ScriptComponent::ScriptComponent(const std::string& script) {
        mScript = script;
    }
    ScriptComponent::ScriptComponent(const ScriptComponent& script) {
        mScript = script.mScript;
    }

    void ScriptComponent::setScript(const std::string& script) {
        mScript = script;
    }

    void ScriptComponent::init(sol::state& lua, Entity ent)
    {
        auto script = AssetManager::get()->getAsset<Script>(mScript);
        if (script) {
            mEnv = sol::environment(lua, sol::create, lua.globals());

            auto result = lua.script(script->mBuffer, mEnv);
            if (result.valid()) {
                mEnv["entity"] = Entity{ ent.mId, ent.mScene };
                if (mEnv["init"] != sol::nil) {
                    mEnv["init"]();
                    isLoaded = true;
                }
            }
        }
    }

    void ScriptComponent::update(float dt) {
        if (isLoaded) {
            mEnv["update"](dt);
        }
    }

    void ScriptComponent::CollisionStart(uint32_t entId, bool isSensor) {
        if (isLoaded && mEnv["collisionStart"] != sol::nil) {
            mEnv["collisionStart"](entId, isSensor);
        }
    }
    void ScriptComponent::CollisionEnd(uint32_t entId, bool isSensor) {
        if (isLoaded && mEnv["collisionEnd"] != sol::nil) {
            mEnv["collisionEnd"](entId, isSensor);
        }
    }

    void ScriptComponent::destroy() {
        if (isLoaded) {
            mEnv["destroy"]();
        }
        mEnv = sol::environment();
        mScript = "";
        isLoaded = false;
    }

} // namespace Plutus