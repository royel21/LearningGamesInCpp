#include "AssetsWindow.h"

#include <memory>
#include <imgui.h>
#include <filesystem>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>
#include <Assets/Assets.h>

#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include "../Config.h"

#include <misc/cpp/imgui_stdlib.h>

#define TreeNodeLeaf_NoPushOpen 264
#define FA_FILE ICON_FA_FILE_ALT " "

#ifdef _WIN32
#define SEPARATOR '\\'
#elif
#define SEPARATOR '/'
#endif

boolmap imgTypes;
boolmap fontTypes;
boolmap soundsTypes;

umap<std::string, std::string> icons;

namespace Plutus
{
    void drawTexture(const Texture& tex, float width);

    EnumFilter filters[] = {
        {GL_NEAREST, "Nearest"},
        {GL_LINEAR, "Linear"}
    };

    AssetsWindow::AssetsWindow()
    {
        imgTypes["png"] = true;
        imgTypes["jpg"] = true;
        imgTypes["jpeg"] = true;
        fontTypes["ttf"] = true;
        fontTypes["otf"] = true;
        soundsTypes["ogg"] = true;
        soundsTypes["wav"] = true;

        icons["Textures"] = ICON_FA_IMAGE " ";
        icons["Fonts"] = ICON_FA_FONT " ";
        icons["Sounds"] = ICON_FA_MUSIC " ";


        icons["png"] = ICON_FA_IMAGE " ";
        icons["jpg"] = ICON_FA_IMAGE " ";
        icons["ttf"] = ICON_FA_FONT " ";
        icons["otf"] = ICON_FA_FONT " ";
        icons["ogg"] = ICON_FA_MUSIC " ";
        icons["wav"] = ICON_FA_MUSIC " ";
    }

    std::string AssetsWindow::getIcon(boolmap& nodes, std::string name)
    {
        std::string icon = nodes[name] ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER;
        return icon + " " + name;
    }

    void audioPlay(Sound* sound, const std::string& name, float width = 0) {
        static bool loop = false;
        if (!width)
            width = ImGui::GetContentRegionAvailWidth() * 0.3f;

        ImGui::Text("Loop");
        ImGui::SameLine(width);
        ImGui::Checkbox("##loop", &loop);
        ImGui::Separator();
        ImGui::Text("Play");
        ImGui::SameLine(width);
        bool state = sound->getState() != 1;
        if (ImGui::TransparentButton(state ? ICON_FA_PLAY : ICON_FA_STOP, true, { 0,0,1,1 })) {
            state ? sound->play(loop) : sound->stop();
        }
        ImGui::SameLine();
        ImGui::Text(name.c_str());
    }

    void AssetsWindow::drawFilter()
    {
        ImGui::BeginUIGroup(ImGuiTableFlags_SizingStretchProp);
        ImGui::BeginCol("Filter");
        ImGui::InputString("##a-filter", filter);
        ImGui::EndUIGroup();
        ImGui::Separator();
    }

    void AssetsWindow::draw()
    {
        if (ImGui::Begin("Assets")) {
            auto mAsset = AssetManager::get();
            if (std::filesystem::exists("./assets/")) {
                if (ImGui::BeginTabBar("##TabBar"))
                {

                    if (ImGui::BeginTabItem("Disk Assets"))
                    {
                        drawFilter();
                        if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                            drawDiskAssets("./assets/");
                        }
                        ImGui::EndChild();
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Scene Assets"))
                    {
                        drawFilter();
                        if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                            int id = 0;
                            drawTreeNode<Font>("Fonts", id);
                            drawTreeNode<Texture>("Textures", id);
                            drawTreeNode<Sound>("Sounds", id);
                        }
                        ImGui::EndChild();
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }

            }
        }
        ImGui::End();

        processFile();
    }

    template<typename T>
    void AssetsWindow::drawTreeNode(const std::string& name, int& id)
    {

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        nodes2[name] = ImGui::TreeNodeEx((void*)(intptr_t)id++, flags, getIcon(nodes2, name).c_str());
        std::string selectedAssetId = "";
        if (nodes2[name]) {
            auto& assets = AssetManager::get()->getAssets<T>();
            static bool show;
            flags |= TreeNodeLeaf_NoPushOpen;
            for (auto asset : assets) {

                if (!filter.empty() && name.find(filter) == std::string::npos) continue;

                ImGui::TreeNodeEx((void*)(intptr_t)id++, flags, (icons[name] + asset.first).c_str());

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    assetType = { asset.first, name };
                    show = true;
                }
                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem((ICON_FA_TRASH_ALT" Remove " + asset.first).c_str()))
                    {
                        selectedAssetId = asset.first;
                    }
                    ImGui::EndPopup();
                }
            }
            if (!selectedAssetId.empty()) {
                assets.erase(selectedAssetId);
            }

            if (show) viewAssets(show);
            ImGui::TreePop();
        }

    }

    void AssetsWindow::viewAssets(bool& show) {
        ImGui::BeginDialog("Asset Modal");
        ImGui::Text(assetType.id.c_str());
        ImGui::Separator();
        auto sound = AssetManager::get()->getAsset<Sound>(assetType.id);
        if (assetType.type.compare("Textures") == 0) {
            auto texture = AssetManager::get()->getAsset<Texture>(assetType.id);
            showTexure(*texture);
        }

        if (assetType.type.compare("Fonts") == 0) {
        }

        if (assetType.type.compare("Sounds") == 0) {
            audioPlay(sound, assetType.id);
        }

        ImGui::Separator();
        ImGui::EndDialog(show);
        if (!show) {
            if (sound && sound->getState() == 1) sound->stop();
        }
    }

    void AssetsWindow::drawDiskAssets(std::string path)
    {
        uint32_t id = 1;
        for (auto& entry : std::filesystem::directory_iterator(path))
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
            auto path = entry.path().string();
            auto name = entry.path().filename().string();

            if (entry.is_directory())
            {
                nodes[name] = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, getIcon(nodes, name).c_str());

                if (nodes[name]) {
                    drawDiskAssets(path);
                    ImGui::TreePop();
                }
            }
            else
            {
                if (!filter.empty() && name.find(filter) == std::string::npos) continue;

                auto icon = icons[Utils::getExtension(name)];
                icon = icon.empty() ? FA_FILE : icon;
                flags |= TreeNodeLeaf_NoPushOpen;
                if (ImGui::TreeNodeEx((void*)(intptr_t)id, flags, (icon + name).c_str())) {
                    bool isScene = path.find("scene") != std::string::npos;
                    if (ImGui::BeginPopupContextItem())
                    {
                        if (isScene && ImGui::MenuItem(("Add Scene '" + name + "' to project").c_str())) {
                            mConfig->mProject.add(path.c_str());
                        }

                        if (!isScene && ImGui::MenuItem(("Add " + name + " to scene").c_str())) {
                            std::replace(path.begin(), path.end(), '\\', '/');
                            selectedDir = path;
                            assetType = { name, path };
                            assetId = name;
                        }
                        ImGui::EndPopup();
                    }
                }
            }
            id++;
        }
    }

    void AssetsWindow::processFile()
    {
        static int fontSize = 16;
        ImVec2 vec = { 1,1 };

        int type = 0;

        if (!selectedDir.empty()) {
            float width = ImGui::GetContentRegionAvailWidth() * 0.3f;

            auto ex = Utils::getExtension(selectedDir);

            size_t start = 9;
            size_t end = selectedDir.find_first_of("/", start);
            auto substr = selectedDir.substr(start, end - start);

            bool show = true;

            ImGui::BeginDialog(("Asset Modal - " + assetType.id).c_str());
            //Texure Or Font Id
            ImGui::Text("Asset Id");
            ImGui::SameLine(width);
            ImGui::InputText("##asset-modal", &assetId);
            ImGui::Separator();

            if (substr.compare("textures") == 0 && imgTypes[ex]) {

                if (texture.mTexId == -1) {
                    texture.init(selectedDir, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                }
                type = 1;
                showTexure(texture, true, width);
            }

            if (substr.compare("fonts") == 0 && fontTypes[ex]) {
                ImGui::Row("Font Size", width);
                if (ImGui::InputInt("##Font-Size", &fontSize)) {
                    mFont.destroy();
                }
                ImGui::Separator();
                if (mFont.mTexId == -1) {
                    mFont.init(selectedDir, fontSize);

                    texture.mTexId = mFont.mTexId;
                    texture.mWidth = 500;
                    texture.mHeight = 500;
                }

                drawTexture(texture, width);
                type = 2;
            }

            if (substr.compare("sounds") == 0 && soundsTypes[ex]) {
                type = 3;
                if (mSound.mPath.empty()) {
                    mSound.init(selectedDir);
                }
                audioPlay(&mSound, assetType.id, width);
            }

            ImGui::Separator();
            if (ImGui::Button("save##modal") && assetId.length() > 0)
            {
                switch (type) {
                case 1:
                    AssetManager::get()->addAsset<Texture>(assetId, selectedDir, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                    break;
                case 2:
                    AssetManager::get()->addAsset<Font>(assetId, selectedDir, fontSize);
                    break;
                case 3:
                    AssetManager::get()->addAsset<Sound>(assetId, selectedDir, 0);
                    break;
                }
                show = false;
            }
            ImGui::EndDialog(show);

            if (!show) {
                selectedDir = "";
                assetId = "";
                fontSize = 20;
                texture.destroy();
                mSound.destroy();
                mFont.destroy();
            }
        }

    }

    void AssetsWindow::showTexure(Texture& texture, bool newTex, float width) {
        if (!width)
            width = ImGui::GetContentRegionAvailWidth() * 0.3f;

        {
            ImGui::Row("Tile Width", width);
            if (ImGui::InputInt("##Tile Width", &texture.mTileWidth))  texture.calculateUV();

            ImGui::Row("Tile Height", width);
            if (ImGui::InputInt("##Tile Height", &texture.mTileHeight)) texture.calculateUV();
            if (newTex) {
                ImGui::Row("Filters", width);
                if (ImGui::BeginCombo("##Filters", texfilter.Name))
                {
                    for (int i = 0; i < IM_ARRAYSIZE(filters); i++) {
                        bool is_selected = filters[i].filter == texfilter.filter;
                        if (ImGui::Selectable(filters[i].Name, is_selected)) {
                            texfilter = filters[i];

                            if (texture.mWidth) {
                                texture.mWidth = 0;
                                texture.destroy();
                            }

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
            }

        }

        drawTexture(texture, width);
    }

    void drawTexture(const Texture& tex, float width) {
        static float scale = 1.0f;

        ImGui::Row("Scale", width);
        ImGui::DragFloat("##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
        ImGui::Separator();
        std::vector<vec3i> selected;
        ImGui::DrawTexture(&tex, 400, 300, scale, &selected);
    }
}
