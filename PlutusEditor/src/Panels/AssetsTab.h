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
    class EditorUI;

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
        void drawTreeNode(std::string dirpath);
        void setParent(EditorUI* parent);

    private:
        void processFile();
        std::string getIcon(boolmap& nodes, const std::string name);

        template<typename T>
        void drawTreeNode(std::string name, T& assets, int& id);
        void addSound2Scene();
        void showTexure(Texture& texture);
        void viewAssets(bool& show);

    private:
        Scene* mScene;
        AudioEvent* aEvent = nullptr;
        std::string selectedDir;
        std::string assetId;
        boolmap nodes;
        boolmap nodes2;
        Texture texture;
        EnumFilter filter = { GL_NEAREST, "Nearest" };
        AssetType assetType;
        EditorUI* parent;
    };

} // namespace Plutus
