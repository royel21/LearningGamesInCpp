#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include <unordered_map>

#include "Asset.h"

#define umap std::unordered_map

struct ma_engine;

namespace Plutus
{
    struct Sound;

    class AssetManager
    {
    private:
        std::string baseDir = "";
        ma_engine* mAudioEngine = nullptr;
        umap<const std::type_info*, umap<std::string, Asset*>> mAssets;

        friend Sound;

    public:
        static AssetManager* get();

        ~AssetManager();

        void setBaseDir(const std::string& dir) { baseDir = dir; }
        std::string getBaseDir() { return baseDir; }

        template <typename T, typename... TArgs>
        T* addAsset(const std::string& id, TArgs &&... args)
        {
            T* asset = new T();
            auto& repo = mAssets[&typeid(T)];
            repo[id] = asset;
            asset->setDir(baseDir);
            asset->init(std::forward<TArgs>(args)...);
            return asset;
        }

        template<typename T>
        T* getAsset(const std::string& id) {
            return hasAsset<T>(id) ? static_cast<T*>(mAssets[&typeid(T)][id]) : nullptr;
        }

        template<typename T>
        bool hasAsset(const std::string id) {
            auto& repo = mAssets[&typeid(T)];
            return repo.find(id) != repo.end();
        }

        template<typename T>
        void removeAsset(const std::string& id) {
            auto& repo = mAssets[&typeid(T)];

            auto it = repo.find(id);
            if (it != repo.end()) {
                it->second->destroy();
                delete it->second;
                repo.erase(it);
            }
        }

        template<typename T>
        umap<std::string, Asset*>& getAssets() {
            return mAssets[&typeid(T)];
        }

        void destroy();

    private:
        AssetManager();
    };
} // namespace Plutus