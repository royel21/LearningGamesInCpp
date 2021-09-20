#include "Script.h"
#include "ECS/ScriptServer.h"
#include <lua.h>

namespace Plutus
{

    Script::Script(std::string _path, Entity* ent, Scene* scene) : path(_path)
    {
        auto lua = ScriptServer::get()->getState();

        mEnv = sol::environment(*lua, sol::create, lua->globals());
        mEnv[ent->getName()] = ent;

        lua->do_file(_path, mEnv);

        mEnv["init"]();
    }

} // namespace Plutus