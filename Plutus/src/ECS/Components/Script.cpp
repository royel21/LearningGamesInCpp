#include "Script.h"
#include "../ScriptServer.h"

#include <lua.h>

namespace Plutus
{
    Script::Script(const Script& script) {
        init(script.path, script.ent, script.scene);
    }

    void Script::init(const std::string& _path, Entity _ent, Scene* _scene)
    {
        path = _path;
        ent = _ent;
        scene = _scene;

        auto lua = ScriptServer::get()->getState();

        mEnv = sol::environment(*lua, sol::create, lua->globals());
        mEnv[ent.getName()] = &ent;

        lua->do_file(_path, mEnv);

        if (mEnv["init"] != sol::nil) {
            mEnv["init"]();
        }
    }

} // namespace Plutus