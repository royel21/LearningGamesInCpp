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
#include <Log/Logger.h>

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

    bool EnumTypeCombo(const char* id, const std::vector<EnumFilter>& items, int& selected) {
        bool change = false;
        if (ImGui::BeginCombo("##Filters", items[selected].Name)) {
            for (size_t i = 0; i < items.size(); i++) {
                bool is_selected = i == selected;
                if (ImGui::Selectable(items[i].Name, is_selected)) {
                    selected = i;
                    change = true;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        return change;
    }

    void drawTexture(const Texture& tex, float width);

    std::vector<EnumFilter> filters = {
        {GL_NEAREST, "Nearest"},
        {GL_LINEAR, "Linear"}
    };

    std::vector<EnumFilter> SoundTypeMaps = {
        {0, "EFFECT" },
        {1, "MUSIC"}
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
        soundsTypes["mp3"] = true;

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

    void AssetsWindow::fileDrop(const std::string& file)
    {
        dropFile = file;
        assetId = Utils::getFileName(file);
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

        ImGui::Row("Loop", width);
        ImGui::Checkbox("##loop", &loop);
        ImGui::Separator();
        ImGui::Row("Play", width);
        bool state = sound->getState() != 1;
        if (ImGui::TransparentButton(state ? ICON_FA_PLAY : ICON_FA_STOP, true, { 0,0,1,1 })) {
            state ? sound->play(loop) : sound->stop();
        }
        ImGui::SameLine();
        ImGui::Text(name.c_str());
    }

    void AssetsWindow::draw()
    {
        if (ImGui::Begin("Assets")) {
            auto mAsset = AssetManager::get();
            if (std::filesystem::exists("./assets/")) {

                ImGui::Row("Filter");
                ImGui::InputString("##a-filter", filter);
                ImGui::Separator();

                if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                    drawTreeNode<Font>("Fonts");
                    drawTreeNode<SceneAsset>("Scenes");
                    drawTreeNode<Script>("Scripts");
                    drawTreeNode<Sound>("Sounds");
                    drawTreeNode<Texture>("Textures");
                }
                ImGui::EndChild();

            }
        }
        ImGui::End();

        processFile();
    }

    template<typename T>
    void AssetsWindow::drawTreeNode(const std::string& name)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        nodes2[name] = ImGui::TreeNodeEx(name.c_str(), flags, getIcon(nodes2, name).c_str());
        std::string selectedAssetId = "";
        if (nodes2[name]) {
            auto& assets = AssetManager::get()->getAssets<T>();
            static bool show;
            flags |= TreeNodeLeaf_NoPushOpen;
            for (auto asset : assets) {

                if (!filter.empty() && name.find(filter) == std::string::npos) continue;

                ImGui::TreeNodeEx(asset.first.c_str(), flags, (icons[name] + asset.first).c_str());

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

    void AssetsWindow::processFile()
    {
        if (!dropFile.empty()) {
            std::string asset = "";
            auto name = Utils::getFileName(dropFile);

            static int fontSize = 16;
            ImVec2 vec = { 1,1 };

            int type = 0;
            static int soundType = 0;

            float width = ImGui::GetContentRegionAvailWidth() * 0.3f;

            auto ex = Utils::getExtension(dropFile);

            bool show = true;

            ImGui::BeginDialog(("Asset Modal - " + assetType.id).c_str());
            //Texure Or Font Id
            ImGui::Row("Asset Id", width);
            ImGui::InputText("##asset-modal", &assetId);
            ImGui::Separator();

            if (imgTypes[ex]) {

                if (texture.mTexId == -1) {
                    texture.init(dropFile, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                }
                type = 1;
                showTexure(texture, true, width);

                asset = "assets\\textures\\" + name;
            }

            if (fontTypes[ex]) {
                ImGui::Row("Font Size", width);
                if (ImGui::InputInt("##Font-Size", &fontSize)) {
                    mFont.destroy();
                }
                ImGui::Separator();
                if (mFont.mTexId == -1) {
                    mFont.init(dropFile, fontSize);

                    texture.mTexId = mFont.mTexId;
                    texture.mWidth = 500;
                    texture.mHeight = 500;
                }

                drawTexture(texture, width);
                type = 2;
                asset = "assets\\fonts\\" + name;
            }

            if (soundsTypes[ex]) {
                type = 3;
                if (mSound.mPath.empty()) {
                    mSound.init(dropFile);
                }
                ImGui::Row("Type", width);
                EnumTypeCombo("###sound-types", SoundTypeMaps, soundType);
                audioPlay(&mSound, assetType.id, width);

                asset = "assets\\sounds\\" + name;
            }

            ImGui::Separator();
            if (ImGui::Button("save##modal") && assetId.length() > 0)
            {
                std::string baseDir = mConfig->mProject.workingDir;
                std::string dest = FileIO::joinPath(baseDir, asset);

                Utils::replaceAll(asset, '\\', '/');
                if (FileIO::copyFile(dropFile, dest))
                {
                    switch (type)
                    {
                    case 1:
                    {
                        AssetManager::get()->addAsset<Texture>(assetId, asset, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                        break;
                    }
                    case 2:
                    {
                        AssetManager::get()->addAsset<Font>(assetId, asset, fontSize);
                        break;
                    }
                    case 3:
                    {
                        AssetManager::get()->addAsset<Sound>(assetId, asset, soundType);
                        break;
                    }
                    }
                }

                show = false;
            }
            ImGui::EndDialog(show);

            if (!show) {
                dropFile = "";
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
                static int selected = 0;
                ImGui::Row("Filters", width);
                if (EnumTypeCombo("##filters", filters, selected)) {
                    texfilter = filters[selected];

                    if (texture.mWidth) {
                        texture.mWidth = 0;
                        texture.destroy();
                    }
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
