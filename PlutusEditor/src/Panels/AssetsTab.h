#pragma once
#include <string>
#include <unordered_map>

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
        bool addTexture();
        bool addFont();
    private:
        Scene* mScene;
        std::string selectedDir;
        std::unordered_map<std::string, bool> nodes;
    };

} // namespace Plutus
