#pragma once
#include <string>
#include <unordered_map>
#include <Assets/Assets.h>

#define umap std::unordered_map
#define boolmap umap<std::string, bool>

namespace Plutus
{
    class Scene;
    struct Config;

    struct EnumFilter {
        GLint filter;
        char* Name;
    };

    struct AssetType {
        std::string id;
        std::string type;
    };

    class AssetsWindow
    {
    private:
        Config* mConfig;
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

    public:
        AssetsWindow();
        void init(Config* config) { mConfig = config; }
        void draw();

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
    };

} // namespace Plutus
