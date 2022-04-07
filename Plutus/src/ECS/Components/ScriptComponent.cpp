#include "ScriptComponent.h"

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
        mEnv = sol::environment(lua, sol::create, lua.globals());

        lua.do_file(mScript, mEnv);

        mEnv[ent.getName()] = Entity{ ent.mId, ent.mScene };
        if (mEnv["init"] != sol::nil) {
            mEnv["init"]();
        }
    }

    void ScriptComponent::destroy() {
        if (mEnv["destroy"] != sol::nil) {
            mEnv["destroy"]();
        }
        mEnv = sol::environment();
        mScript = "";
    }

} // namespace Plutus