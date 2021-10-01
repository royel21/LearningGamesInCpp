#include "Script.h"
#include "ECS/ScriptServer.h"
#include <lua.h>

namespace Plutus
{
    Script::Script(std::string _path, Entity& ent, Scene* scene)
    {
        auto lua = ScriptServer::get()->getState();

        mEnv = sol::environment(*lua, sol::create, lua->globals());
        mEnv[ent.getName()] = new Entity{ ent.mId, ent.mScene };

        lua->do_file(_path, mEnv);

        mEnv["init"]();
    }

} // namespace Plutus