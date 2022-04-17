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
        ma_engine* mAudioEngine = nullptr;
        umap<const std::type_info*, umap<std::string, Asset*>> mAssets;

        friend Sound;

    public:
        static AssetManager* get();

        ~AssetManager();

        template <typename T, typename... TArgs>
        T* addAsset(const std::string& id, TArgs &&... args)
        {
            T* asset = new T(std::forward<TArgs>(args)...);
            auto& repo = mAssets[&typeid(T)];
            repo[id] = asset;
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
        void removeAsset(std::string id) {
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