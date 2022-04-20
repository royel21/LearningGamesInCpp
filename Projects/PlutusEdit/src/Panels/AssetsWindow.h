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
        int filter;
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
        Texture texture;
        Font mFont;

        std::string assetId;
        std::string dropFile = "";

        EnumFilter texfilter = { GL_NEAREST, "Nearest" };

        AssetType assetType;
        std::string filter;

        boolmap nodes;
        boolmap nodes2;

    public:
        AssetsWindow();
        void init(Config* config) { mConfig = config; }

        void fileDrop(const std::string& file);
        void draw();

    private:
        void processFile();

        std::string getIcon(boolmap& nodes, const std::string name);

        template<typename T>
        void drawTreeNode(const std::string& name);

        void addSound2Scene();

        void showTexure(Texture& texture, bool newTex = false, float width = 0);

        void viewAssets(bool& show);

        void drawFilter();
    };

} // namespace Plutus
