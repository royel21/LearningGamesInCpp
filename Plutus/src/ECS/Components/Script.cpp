#include "Script.h"
#include "../ScriptServer.h"

#include <lua.h>

namespace Plutus
{
    Script::Script(const std::string& script) {
        init(script);
    }
    Script::Script(const Script& script) {
        init(script.mScript);
    }

    void Script::init(const std::string& _script)
    {
        if (!_script.empty()) mScript = _script;

        auto lua = ScriptServer::get()->getState();

        mEnv = sol::environment(*lua, sol::create, lua->globals());

        lua->do_file(mScript, mEnv);

        if (mEnv["init"] != sol::nil) {
            mEnv["init"]();
        }
    }

    void Script::destroy() {
        mEnv = sol::environment();
    }

} // namespace Plutus