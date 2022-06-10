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
    public:
        static AssetManager* get();

        ~AssetManager();

        void setBaseDir(const std::string& dir) { baseDir = dir; }
        std::string getBaseDir() { return baseDir; }

        template <typename T, typename... TArgs>
        T* addAsset(const std::string& id, TArgs &&... args)
        {
            auto listId = getListId<T>();
            if (!hasAssetList<T>()) mAssets.resize(listId + 1);

            auto& repo = mAssets[listId];

            if (hasAsset<T>(id)) {
                return static_cast<T*>(repo[id]);
            }
            else {
                T* asset = new T();
                repo[id] = asset;
                asset->setDir(baseDir);
                asset->init(std::forward<TArgs>(args)...);
                return asset;
            }
        }

        template<typename T>
        T* getAsset(const std::string& id) {

            if (hasAssetList<T>()) {
                auto listId = getListId<T>();
                auto found = mAssets[listId].find(id);
                if (found != mAssets[listId].end()) {
                    return static_cast<T*>(found->second);
                }
            }
            return nullptr;
        }

        template<typename T>
        inline bool hasAsset(const std::string& id) {
            if (hasAssetList<T>()) {
                auto& list = mAssets[getListId<T>()];
                return list.find(id) != list.end();
            }

            return false;
        }

        template<typename T>
        inline bool hasAssetList() {
            return getListId<T>() < mAssets.size();
        }

        template<typename T>
        void removeAsset(const std::string& id) {
            if (hasAssetList<T>()) {
                auto& repo = mAssets[getListId<T>()];
                auto it = repo.find(id);
                if (it != repo.end()) {
                    it->second->destroy();
                    delete it->second;
                    repo.erase(it);
                }
            }

        }

        template<typename T>
        umap<std::string, Asset*>& getAssets() {
            auto listId = getListId<T>();
            if (!hasAssetList<T>()) mAssets.resize(listId + 1);
            return mAssets[listId];
        }

        void destroy();

    private:
        std::string baseDir = "";
        ma_engine* mAudioEngine = nullptr;
        std::vector<umap<std::string, Asset*>> mAssets;

        friend Sound;

        AssetManager();

        inline uint32_t getId() {
            static int id = 0;
            return id++;
        }

        template<typename T>
        inline uint32_t getListId()
        {
            static int id = getId();
            return id;
        }
    };
} // namespace Plutus