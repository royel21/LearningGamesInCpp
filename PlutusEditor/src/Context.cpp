#include "Context.h"
#include <Serialize/SceneLoader.h>
#include <Serialize/SceneSerializer.h>
#include <Serialize/Serialize.h>

namespace Plutus
{

    AppContext::AppContext()
    {
        NewScene();
    }

    AppContext& AppContext::get()
    {
        static AppContext context;
        return context;
    }

    void AppContext::NewScene()
    {
        mEnt = {};
        mScene = std::make_shared<Scene>();
    }

    void AppContext::Load(const char* filePath)
    {
        SceneLoader::loadFromJson(filePath, mScene);
    }

    void AppContext::Save(const char* filePath, Ref<Scene>& mScene)
    {
        auto buffer = SceneSerializer(mScene);
        std::string json = Plutus::SceneSerializer(mScene);
        toJsonFile(filePath, json.c_str());
    }

    AppContext& Context = AppContext::get();;
}