#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include <unordered_map>

#include "Asset.h"

#define umap std::unordered_map

namespace Plutus
{
    class AssetManager2
    {
    public:
        ~AssetManager2() = default;

        static AssetManager2* get();

        template <typename T, typename... TArgs>
        T* addAsset(const std::string& id, TArgs &&... args)
        {
            if (!hasAsset<T>(id)) {
                T* asset = new T(std::forward<TArgs>(args)...);
                auto& repo = mAssets[&typeid(T)];
                repo[id] = asset;
                return asset;
            }
            return getAsset<T>(id);
        }

        template<typename T>
        T* getAsset(const std::string& id) {
            return hasAsset<T>(id) ? static_cast<T*>(mAssets[&typeid(T)][id]) : nullptr;
        }

        template<typename T>
        bool hasAsset(const std::string id) {
            auto& repo = mAssets.find(&typeid(T));
            if (repo != mAssets.end()) {
                return repo->second.find(id) != repo->second.end();
            }
            return false;
        }

        template<typename T>
        void removeAsset(std::string id) {
            auto& repo = mAssets.find(&typeid(T));
            if (repo != mAssets.end()) {
                auto it = repo->second.find(id);
                if (it != repo->second.end()) {
                    it->second->destroy();
                    delete it->second;
                    repo->second.erase(it);
                }
            }
        }

        template<typename T>
        umap<std::string, Asset*>* getAssets() {
            auto repo = mAssets.find(&typeid(T));
            if (repo != mAssets.end()) {
                return &repo->second;
            }
            return nullptr;
        }

        void destroy();


    private:
        AssetManager2() = default;
        umap<const std::type_info*, umap<std::string, Asset*>> mAssets;
    };
} // namespace Plutus