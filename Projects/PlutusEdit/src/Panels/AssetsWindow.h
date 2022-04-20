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

    struct AssetFile {
        std::string id;
        std::string name;
        std::string fullpath;
        int type = 0;
        bool isNew = false;
    };

    class AssetsWindow
    {
    private:
        Config* mConfig;
        Scene* mScene;

        Sound mSound;
        Texture texture;
        Font mFont;
        Script mScript;
        SceneAsset mSceneAsset;

        std::string assetId;

        EnumFilter texfilter = { GL_NEAREST, "Nearest" };

        AssetFile assetFile;
        std::string filter;

        boolmap nodes;
        boolmap nodes2;

        std::unordered_map<std::string, int> fileTypes;

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

        void showFont(Font* font, float width);
    };

} // namespace Plutus
