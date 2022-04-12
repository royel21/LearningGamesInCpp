#pragma once
#include <string>
#include <unordered_map>
#include <Assets/Assets.h>

#define umap std::unordered_map
#define boolmap umap<std::string, bool>

namespace Plutus
{
    class Scene;

    struct EnumFilter {
        GLint filter;
        char* Name;
    };

    struct AssetType {
        std::string id;
        std::string type;
    };

    class AssetsTab
    {
    public:
        AssetsTab();
        void drawAssets();

    private:
        void drawDiskAssets(std::string dirpath);

        void processFile();

        std::string getIcon(boolmap& nodes, const std::string name);

        template<typename T>
        void drawTreeNode(const std::string& name, int& id);

        void addSound2Scene();

        void showTexure(Texture& texture, bool newTex = false);

        void viewAssets(bool& show);

        void drawFilter();

    private:
        Scene* mScene;
        Sound mSound;
        std::string selectedDir;
        std::string assetId;
        boolmap nodes;
        boolmap nodes2;
        Texture texture;
        EnumFilter texfilter = { GL_NEAREST, "Nearest" };
        AssetType assetType;
        std::string filter;
    };

} // namespace Plutus
