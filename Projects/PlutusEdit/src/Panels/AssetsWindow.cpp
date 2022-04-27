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

#define FONTS 0
#define TEXURES 1
#define SOUNDS 2
#define SCRIPTS 3
#define SCENES 4

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
        assetMang = AssetManager::get();
        icons["Textures"] = ICON_FA_IMAGE " ";
        icons["Fonts"] = ICON_FA_FONT " ";
        icons["Sounds"] = ICON_FA_MUSIC " ";

        icons["png"] = ICON_FA_IMAGE " ";
        icons["jpg"] = ICON_FA_IMAGE " ";
        icons["jpeg"] = ICON_FA_IMAGE " ";

        icons["ttf"] = ICON_FA_FONT " ";
        icons["otf"] = ICON_FA_FONT " ";
        icons["ogg"] = ICON_FA_MUSIC " ";
        icons["wav"] = ICON_FA_MUSIC " ";

        fileTypes["ttf"] = FONTS;
        fileTypes["otf"] = FONTS;

        fileTypes["ogg"] = SOUNDS;
        fileTypes["wav"] = SOUNDS;
        fileTypes["mp3"] = SOUNDS;

        fileTypes["png"] = TEXURES;
        fileTypes["jpg"] = TEXURES;
        fileTypes["jpeg"] = TEXURES;

        fileTypes["lua"] = SCRIPTS;

        fileTypes["json"] = SCENES;
    }

    void AssetsWindow::fileDrop(const std::string& file)
    {
        auto ex = Utils::getExtension(file);
        if (!mConfig->mProject || ex == "json") {
            assetFile.isNew = true;
            assetFile.id = Utils::getFileName(file);
            assetFile.name = assetFile.id;
            assetFile.fullpath = file;
            assetFile.type = fileTypes[Utils::getExtension(file)];
        }
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

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem((ICON_FA_FOLDER_OPEN" Open Dir: " + name).c_str()))
            {
                FileIO::openDir(mConfig->mProject.getDir(name));
            }
            ImGui::EndPopup();
        }

        std::string selectedAssetId = "";
        if (nodes2[name]) {
            auto& assets = assetMang->getAssets<T>();
            static bool show;
            flags |= TreeNodeLeaf_NoPushOpen;
            for (auto asset : assets) {

                if (!filter.empty() && name.find(filter) == std::string::npos) continue;

                ImGui::TreeNodeEx(asset.first.c_str(), flags, (icons[name] + asset.first).c_str());

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    assetFile.name = asset.first;
                    assetFile.type = fileTypes[Utils::getExtension(asset.second->mPath)];
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
        ImGui::BeginDialog("Asset Modal", false, { 450.0f, 0.0f });
        float width = ImGui::GetContentRegionAvailWidth() * 0.3f;
        ImGui::Row("Id", width);
        ImGui::Text(assetFile.name.c_str());
        ImGui::Separator();
        switch (assetFile.type)
        {
        case FONTS:
        {
            auto font = assetMang->getAsset<Font>(assetFile.name);
            if (font) {
                showFont(font, width);
            }
            break;
        }
        case TEXURES:
        {
            auto tex = assetMang->getAsset<Texture>(assetFile.name);
            if (tex) {
                showTexure(*tex);
            }
            break;
        }
        case SOUNDS:
        {
            auto sound = assetMang->getAsset<Sound>(assetFile.name);
            if (sound) {
                ImGui::Row("Type", width);
                EnumTypeCombo("###sound-types", SoundTypeMaps, mSound.mType);
                audioPlay(sound, assetFile.name, width);
            }
            break;
        }
        case SCRIPTS:
        {
            break;
        }
        case SCENES: {
            break;
        }
        }
        ImGui::Separator();
        ImGui::EndDialog(show);
        if (!show) {
            texture.destroy();
            assetFile.name = "";
            assetFile.fullpath = "";
        }
    }

    void AssetsWindow::processFile()
    {
        bool show = true;
        if (!assetFile.fullpath.empty()) {
            std::string asset = "";

            float width = ImGui::GetContentRegionAvailWidth() * 0.3f;


            ImGui::BeginDialog(("Asset Modal - " + assetFile.name).c_str(), false, { 450.0f, 0.0f });
            //Texure Or Font Id
            ImGui::Row("Asset Id", width);
            ImGui::InputText("##asset-modal", &assetFile.id);
            ImGui::Separator();

            switch (assetFile.type)
            {
            case FONTS:
            {
                mFont.mPath = assetFile.fullpath;
                showFont(&mFont, width);
                asset = "assets\\fonts\\" + assetFile.name;
                break;
            }
            case TEXURES:
            {
                if (texture.mTexId == -1) {
                    texture.init(assetFile.fullpath, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                }
                showTexure(texture, true, width);

                asset = "assets\\textures\\" + assetFile.name;
                break;
            }
            case SOUNDS:
            {
                if (mSound.mPath.empty()) {
                    mSound.init(assetFile.fullpath);
                }
                ImGui::Row("Type", width);
                EnumTypeCombo("###sound-types", SoundTypeMaps, mSound.mType);
                audioPlay(&mSound, assetFile.name, width);

                asset = "assets\\sounds\\" + assetFile.name;
                break;
            }
            case SCRIPTS:
            {
                if (mScript.mBuffer.length())
                    mScript.init(assetFile.fullpath);

                asset = "assets\\scripts\\" + assetFile.name;
                break;
            }
            case SCENES:
            {
                if (!mSceneAsset.mPath.length())
                    mSceneAsset.init(assetFile.fullpath);

                asset = "assets\\scenes\\" + assetFile.name;
                break;
            }

            }

            ImGui::Separator();
            ImGui::EndDialog(show, [&](bool save) {
                if (save && assetFile.id.length() > 0)
                {
                    std::string baseDir = mConfig->mProject.workingDir;
                    std::string dest = FileIO::joinPath(baseDir, asset);
                    Utils::replaceAll(asset, '\\', '/');
                    if (FileIO::copyFile(assetFile.fullpath, dest))
                    {
                        switch (assetFile.type)
                        {
                        case FONTS: {
                            assetMang->addAsset<Font>(assetFile.id, asset, mFont.mSize);
                            mFont.destroy();
                            break;
                        }
                        case TEXURES:
                        {
                            assetMang->addAsset<Texture>(assetFile.id, asset, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                            texture.destroy();
                            break;
                        }
                        case SOUNDS:
                        {
                            assetMang->addAsset<Sound>(assetFile.id, asset, mSound.mType);
                            mSound.destroy();
                            break;
                        }
                        case SCRIPTS:
                        {
                            assetMang->addAsset<Script>(assetFile.id, asset);
                            mScript.destroy();
                            break;
                        }
                        case SCENES: {
                            assetMang->addAsset<SceneAsset>(assetFile.id, asset);
                            break;
                        }
                        }
                    }
                }

                assetFile.fullpath = "";
                });
        }

    }

    void AssetsWindow::showTexure(Texture& texture, bool newTex, float width) {
        if (!width)
            width = ImGui::GetContentRegionAvailWidth() * 0.3f;

        ImGui::Row("Tile Width", width);
        if (ImGui::InputInt("##Tile Width", &texture.mTileWidth))  texture.calculateUV();

        ImGui::Row("Tile Height", width);
        if (ImGui::InputInt("##Tile Height", &texture.mTileHeight)) texture.calculateUV();

        ImGui::Row("Spacing", width);
        if (ImGui::InputInt("##t-spacing", &texture.mSpacing)) texture.calculateUV();

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


    void AssetsWindow::showFont(Font* font, float width) {
        ImGui::Row("Font Size", width);
        if (ImGui::InputInt("##Font-Size", &font->mSize)) {
            font->destroy();
        }
        ImGui::Separator();
        if (font->mTexId == -1) {
            font->init(font->mPath, font->mSize);
        }


        texture.mTexId = font->mTexId;
        texture.mWidth = 500;
        texture.mHeight = 500;
        texture.setTilesSize(0, 0);
        texture.mMagFilter = GL_NEAREST;
        texture.mMinFilter = GL_NEAREST;

        drawTexture(texture, width);
        texture.mTexId = -1;
    }
}
