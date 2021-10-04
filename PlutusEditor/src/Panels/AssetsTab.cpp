#include "AssetsTab.h"

#include <memory>
#include <imgui.h>
#include <filesystem>

#include <Utils/FileIO.h>
#include <Assets/AssetManager.h>

#include "../ImGuiEx.h"
#include "../IconsFontAwesome5.h"

#define TreeNodeLeaf_NoPushOpen 264
#define FA_FILE ICON_FA_FILE_ALT " "

#ifdef _WIN32
#define SEPARATOR '\\'
#elif
#define SEPARATOR '/'
#endif

boolmap imgTypes;
boolmap fontTypes;

namespace Plutus
{
    AssetsTab::AssetsTab()
    {
        imgTypes["png"] = true;
        imgTypes["jpg"] = true;
        imgTypes["jpeg"] = true;
        fontTypes["ttf"] = true;
        fontTypes["otf"] = true;
    }

    std::string AssetsTab::getIcon(boolmap& nodes, std::string name)
    {
        std::string icon = nodes[name] ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER;
        return icon + " " + name;
    }

    template<typename T>
    void AssetsTab::drawTreeNode(std::string name, T& assets, int& id)
    {
        nodes2[name] = ImGui::TreeNode((void*)(intptr_t)id++, getIcon(nodes2, name).c_str());
        std::string assetId = "";
        if (nodes2[name]) {
            for (auto asset : assets.getItems()) {
                ImGui::TreeNodeEx((void*)(intptr_t)id++, TreeNodeLeaf_NoPushOpen, (FA_FILE + asset.first).c_str());
                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem(("Remove " + asset.first).c_str()))
                    {
                        assetId = asset.first;
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::TreePop();
            if (!assetId.empty()) {
                assets.removeItem(assetId);
            }
        }
    }

    void AssetsTab::drawAssets()
    {
        ImGui::Begin("Assets");
        auto mAsset = AssetManager::get();
        if (std::filesystem::exists("./assets/")) {
            if (ImGui::BeginTabBar("##TabBar"))
            {
                if (ImGui::BeginTabItem("Assets"))
                {
                    drawTreeNode("./assets/");
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Scene Assets"))
                {
                    int id = 0;
                    drawTreeNode("Fonts", mAsset->mFonts, id);
                    drawTreeNode("Textures", mAsset->mTextures, id);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

        }
        ImGui::End();

        processFile();
    }

    void AssetsTab::drawTreeNode(std::string path)
    {
        uint32_t id = 1;
        for (auto& entry : std::filesystem::directory_iterator(path))
        {
            auto path = entry.path().string();
            auto name = entry.path().filename().string();

            if (entry.is_directory())
            {
                nodes[name] = ImGui::TreeNode((void*)(intptr_t)id, getIcon(nodes, name).c_str());

                if (nodes[name]) {
                    drawTreeNode(path);
                    ImGui::TreePop();
                }
            }
            else
            {
                ImGui::TreeNodeEx((void*)(intptr_t)id, TreeNodeLeaf_NoPushOpen, (FA_FILE + name).c_str());

                int found = path.find("scene");

                if (found > 0 && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {

                }

                if (found < 0 && ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem(("Add " + name + " to scene").c_str()))
                    {
                        selectedDir = path;
                    }
                    ImGui::EndPopup();
                }
            }
            id++;
        }
    }

    void AssetsTab::processFile()
    {
        static char name[128];
        static int tileWidth = 0;
        static int tileHeight = 0;
        static int columns = 0;
        static int fontSize = 20;
        ImVec2 vec = glm::vec2(1);

        bool isFont = false;

        if (!selectedDir.empty()) {
            auto ex = Utils::getExtension(selectedDir);

            int start = 9;
            int end = selectedDir.find_first_of(SEPARATOR, start);
            auto substr = selectedDir.substr(start, end - start);

            bool show = true;
            ImGui::BeginDialog("Asset Modal", 300, 300);
            //Texure Or Font Id
            ImGui::PushItemWidth(100);
            ImGui::InputText("Id##modal", name, IM_ARRAYSIZE(name));

            if (substr.compare("textures") == 0 && imgTypes[ex]) {
                ImGui::InputInt("Columns", &columns);
                ImGui::InputInt("Tile Width", &tileWidth);
                ImGui::InputInt("Tile Height", &tileHeight);

                isFont = false;
            }

            if (substr.compare("fonts") == 0 && fontTypes[ex]) {
                ImGui::InputInt("Font Size", &fontSize);
                isFont = true;
            }
            ImGui::PopItemWidth();
            ImGui::Separator();
            if (ImGui::Button("save##modal"))
            {
                if (isFont) {
                    AssetManager::get()->mFonts.addFont(name, selectedDir, fontSize);
                }
                else {
                    AssetManager::get()->mTextures.addTexture(name, selectedDir, columns, tileHeight, tileWidth);
                }
                show = false;
            }
            ImGui::EndDialog(show);

            if (!show) {
                selectedDir = "";
                memset(name, 0, 128);
                tileWidth = 0;
                tileHeight = 0;
                columns = 0;
                fontSize = 20;
            }
        }

    }
}