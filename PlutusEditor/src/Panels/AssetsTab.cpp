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

std::unordered_map<std::string, bool> imgTypes;
std::unordered_map<std::string, bool> fontTypes;

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

    void AssetsTab::drawAssets()
    {
        ImGui::Begin("Assets");

        ImGui::Text("Assets");
        ImGui::Separator();

        if (std::filesystem::exists("./assets/")) {
            drawTreeNode("./assets/");
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

            if (entry.is_directory()) {
                std::string icon = nodes[name] ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER;
                std::string dirname = icon + " " + name;

                bool opened = ImGui::TreeNode((void*)(intptr_t)id, dirname.c_str());
                nodes[name] = opened;

                if (opened) {
                    drawTreeNode(path);
                    ImGui::TreePop();
                }
            }
            else {
                std::string icon = std::string(ICON_FA_FILE_ALT) + "\xef\x85\x9c " + name;

                ImGui::TreeNodeEx((void*)(intptr_t)id, TreeNodeLeaf_NoPushOpen, (FA_FILE + name).c_str());

                int found = path.find("scene");

                if (found > 0 && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    std::printf("dblck\n");
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

    bool AssetsTab::addFont()
    {
        bool show = true;
        ImGui::BeginDialog("Add Font", 300, 300);
        static char fontId[120];
        // AssetManager::get()->mFonts.addFont();
        ImGui::EndDialog(show);
        return show;
    }

    bool AssetsTab::addTexture()
    {
        bool show = true;
        ImGui::BeginDialog("Add Texture", 300, 300);
        static char texId[120];

        ImGui::Text("Texture");
        ImGui::EndDialog(show);
        return show;
    }
}