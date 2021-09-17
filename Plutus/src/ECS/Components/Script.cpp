#include "Script.h"
#include "ECS/ScriptServer.h"
#include <lua.h>

namespace Plutus
{

    Script::Script(std::string _path, Entity* ent, Scene* scene) : path(_path)
    {
        auto lua = ScriptServer::get()->getState();

        mEnv = sol::environment(*lua, sol::create, lua->globals());

        sol::load_result script = lua->load_file(_path);
    }

} // namespace Plutus