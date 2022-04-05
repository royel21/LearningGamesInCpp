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
        T* addAsset(const std::string& id, TArgs &&... args)
        {
            if (!hasAsset(id)) {
                T* asset = new T(std::forward<TArgs>(args)...);
                mAssets[id] = asset;
                return asset;
            }
            return getAsset<T>(id);
        }

        template<typename T>
        T* getAsset(const std::string& id) {
            return hasAsset(id) ? static_cast<T*>(mAssets[id]) : nullptr;
        }

        bool hasAsset(const std::string id) { return mAssets.find(id) != mAssets.end(); }

        void removeAsset(std::string id) {}

        void destroy();


    private:
        AssetManager2() = default;
        std::unordered_map<std::string, Asset*> mAssets;
    };
} // namespace Plutus