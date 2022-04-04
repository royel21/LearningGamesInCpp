#pragma once
#include <unordered_map>
#include <string>

#include "Asset.h"

namespace Plutus
{
    class AssetManager2
    {
    public:
        ~AssetManager2() = default;


        static AssetManager2* get();

        template <typename T, typename... TArgs>
        T* add(AssetType type, const std::string& id, TArgs &&... args)
        {
            if (mAssets.find(id) == mAsset.end()) {
                T* asset = new T(std::forward<TArgs>(args)...);
                asset.type = type;
                mAssets[id] = asset;
                return asset;
            }
            return get<T>(id);
        }

        template<typename T>
        T* get(std::string id) { return nullptr; }

        void remove(std::string id) {}


    private:
        AssetManager2() = default;
        std::unordered_map<std::string, Asset*> mAssets;
    };
} // namespace Plutus