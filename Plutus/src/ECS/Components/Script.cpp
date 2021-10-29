#include "Script.h"

namespace Plutus
{
    Script::Script(const std::string& script) {
        mScript = script;
    }
    Script::Script(const Script& script) {
        mScript = script.mScript;
    }

    void Script::setScript(const std::string& script) {
        mScript = script;
    }

    void Script::init(sol::state& lua, Entity ent)
    {
        mEnv = sol::environment(lua, sol::create, lua.globals());

        lua.do_file(mScript, mEnv);

        mEnv[ent.getName()] = Entity{ ent.mId, ent.mScene };
        if (mEnv["init"] != sol::nil) {
            mEnv["init"]();
        }
    }

    void Script::destroy() {
        mEnv = sol::environment();
        mScript = "";
    }

} // namespace Plutus