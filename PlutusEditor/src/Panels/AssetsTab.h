#pragma once
#include <string>
#include <unordered_map>

#define umap std::unordered_map
#define boolmap umap<std::string, bool>

namespace Plutus
{
    class Scene;
    class AssetsTab
    {
    public:
        AssetsTab();
        void drawAssets();
        void drawTreeNode(std::string dirpath);

    private:
        void processFile();
        std::string getIcon(boolmap& nodes, const std::string name);

        template<typename T>
        void drawTreeNode(std::string name, T& assets, int& id);

    private:
        Scene* mScene;
        std::string selectedDir;
        boolmap nodes;
        boolmap nodes2;
    };

} // namespace Plutus
