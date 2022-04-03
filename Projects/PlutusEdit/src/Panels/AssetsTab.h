#pragma once
#include <string>
#include <unordered_map>
#include <Assets/Textures.h>

#define umap std::unordered_map
#define boolmap umap<std::string, bool>

namespace Plutus
{
    class Scene;
    class AudioEvent;

    struct EnumFilter {
        GLuint filter;
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
        void drawTreeNode(std::string name, T& assets, int& id);
        void addSound2Scene();
        void showTexure(Texture& texture, bool newTex = false);
        void viewAssets(bool& show);
        void drawFilter();

    private:
        Scene* mScene;
        AudioEvent* aEvent = nullptr;
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
