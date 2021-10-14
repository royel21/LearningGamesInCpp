#include "AssetsTab.h"

#include <memory>
#include <imgui.h>
#include <filesystem>

#include <Utils/FileIO.h>
#include <Assets/AssetManager.h>

#include "../ImGuiEx.h"
#include "../IconsFontAwesome5.h"
#include <Assets/SoundEngine.h>
#include <Utils/FileIO.h>
#include "../EditorUI.h"


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
    EnumFilter filters[] = {
        {GL_NEAREST, "Nearest"},
        {GL_LINEAR, "Linear"}
    };
    void AssetsTab::setParent(EditorUI* _parent) {
        parent = _parent;
    }

    AssetsTab::AssetsTab()
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

    std::string AssetsTab::getIcon(boolmap& nodes, std::string name)
    {
        std::string icon = nodes[name] ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER;
        return icon + " " + name;
    }

    void audioPlay(AudioEvent* event, const char* name) {
        bool state = event->getState() != 1;
        if (ImGui::TransparentButton(state ? ICON_FA_PLAY : ICON_FA_STOP, { 0,0,1,1 })) {
            state ? SoundEngine.play(event) : SoundEngine.stop(event);
        }
        ImGui::SameLine();
        ImGui::Text(name);
    }

    template<typename T>
    void AssetsTab::drawTreeNode(std::string name, T& assets, int& id)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        nodes2[name] = ImGui::TreeNodeEx((void*)(intptr_t)id++, flags, getIcon(nodes2, name).c_str());
        std::string assetId = "";
        if (nodes2[name]) {
            static bool show;
            flags |= TreeNodeLeaf_NoPushOpen;
            for (auto asset : assets.getItems()) {
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
                        assetId = asset.first;
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::TreePop();
            if (!assetId.empty()) {
                assets.remove(assetId);
            }

            if (show) viewAssets(show);
        }
    }

    void AssetsTab::viewAssets(bool& show) {
        ImGui::BeginDialog("Asset Modal", 300, 300);
        ImGui::Text(assetType.id.c_str());
        ImGui::Separator();
        if (assetType.type.compare("Textures") == 0) {
            auto& textures = AssetManager::get()->mTextures;

            auto texture = AssetManager::get()->mTextures.getTexture(assetType.id);
            showTexure(*texture);
        }

        if (assetType.type.compare("Fonts") == 0) {
        }

        if (assetType.type.compare("Sounds") == 0) {
            auto audioItems = SoundEngine.getItems();

            audioPlay(audioItems[assetType.id], assetType.id.c_str());
        }

        ImGui::Separator();
        ImGui::EndDialog(show);
    }

    void AssetsTab::drawAssets()
    {
        if (ImGui::Begin("Assets")) {
            auto mAsset = AssetManager::get();
            if (std::filesystem::exists("./assets/")) {
                if (ImGui::BeginTabBar("##TabBar"))
                {
                    if (ImGui::BeginTabItem("Assets"))
                    {

                        if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                            drawTreeNode("./assets/");
                            ImGui::EndChild();
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Scene Assets"))
                    {
                        if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                            int id = 0;
                            drawTreeNode("Fonts", mAsset->mFonts, id);
                            drawTreeNode("Textures", mAsset->mTextures, id);
                            drawTreeNode("Sounds", SoundEngine, id);
                            ImGui::EndChild();
                        }
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }

            }
            ImGui::End();
        }

        processFile();
    }

    void AssetsTab::drawTreeNode(std::string path)
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
                    drawTreeNode(path);
                    ImGui::TreePop();
                }
            }
            else
            {
                auto icon = icons[Utils::getExtension(name)];
                icon = icon.empty() ? FA_FILE : icon;
                flags |= TreeNodeLeaf_NoPushOpen;
                ImGui::TreeNodeEx((void*)(intptr_t)id, flags, (icon + name).c_str());

                bool isScene = path.find("scene") != std::string::npos;

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    if (isScene) {

                    }
                    if (path.find(".lua") != std::string::npos) {
                        auto script = readFileAsString(path.c_str());
                        parent->setEditorScript(script);
                    }
                }


                if (!isScene && ImGui::BeginPopupContextItem())
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
        static int fontSize = 20;
        ImVec2 vec = glm::vec2(1);

        int type = 0;

        if (!selectedDir.empty()) {
            auto ex = Utils::getExtension(selectedDir);

            size_t start = 9;
            size_t end = selectedDir.find_first_of(SEPARATOR, start);
            auto substr = selectedDir.substr(start, end - start);

            bool show = true;
            ImGui::BeginDialog("Asset Modal", 300, 300);
            //Texure Or Font Id
            ImGui::Text("Asset Id");
            ImGui::SameLine();
            ImGui::InputText("##asset-modal", name, IM_ARRAYSIZE(name));
            ImGui::Separator();

            if (substr.compare("textures") == 0 && imgTypes[ex]) {
                auto& textures = AssetManager::get()->mTextures;

                if (texture.texWidth == 0) {
                    auto glTexture = textures.createGLTexure(selectedDir, filter.filter, filter.filter);
                    texture.texId = glTexture.id;
                    texture.texWidth = glTexture.width;
                    texture.texHeight = glTexture.height;
                }
                type = 1;
                showTexure(texture);
            }

            if (substr.compare("fonts") == 0 && fontTypes[ex]) {
                ImGui::InputInt("Font Size", &fontSize);
                type = 2;
            }

            if (substr.compare("sounds") == 0 && soundsTypes[ex]) {
                type = 3;
                if (!aEvent) {
                    aEvent = SoundEngine.createEvent("temp", selectedDir, EFFECT);
                }
                audioPlay(aEvent, Utils::getFileName(selectedDir).c_str());
            }

            ImGui::Separator();
            if (ImGui::Button("save##modal") && strlen(name) > 0)
            {
                switch (type) {
                case 1:
                    AssetManager::get()->mFonts.addFont(name, selectedDir, fontSize);
                    break;
                case 2:
                    AssetManager::get()->mTextures.addTexture(name, selectedDir, texture.columns, texture.tileHeight, texture.tileWidth, filter.filter, filter.filter);
                    break;
                case 3:
                    SoundEngine.add(name, selectedDir, EFFECT);
                    break;
                }
                show = false;
            }
            ImGui::EndDialog(show);

            if (!show) {
                selectedDir = "";
                memset(name, 0, 128);
                fontSize = 20;
                if (texture.texWidth) {
                    glDeleteTextures(1, &texture.texId);
                }
                texture = {};
                if (aEvent) {
                    delete aEvent;
                    aEvent = nullptr;
                }
            }
        }

    }

    void BeginPro(char* label, float width = -1) {
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(label);
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(width);
    }

    void AssetsTab::showTexure(Texture& texture) {

        static float scale = 1.0f;
        if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingStretchProp)) {
            ImGui::BeginCol("Columns");
            ImGui::InputInt("##Columns", &texture.columns);

            ImGui::BeginCol("Tile Width");
            ImGui::InputInt("##Tile Width", &texture.tileWidth);

            ImGui::BeginCol("Tile Height");
            ImGui::InputInt("##Tile Height", &texture.tileHeight);

            ImGui::BeginCol("Filters");
            if (ImGui::BeginCombo("##Filters", filter.Name))
            {
                for (int i = 0; i < IM_ARRAYSIZE(filters); i++) {
                    bool is_selected = filters[i].filter == filter.filter;
                    if (ImGui::Selectable(filters[i].Name, is_selected)) {
                        filter = filters[i];

                        if (texture.texWidth) {
                            texture.texWidth = 0;
                            glDeleteTextures(1, &texture.texId);
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::BeginCol("Scale");
            ImGui::DragFloat("##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
            ImGui::EndUIGroup();
        }
        ImGui::DrawTexture(&texture, 350, 300, scale);
    }
}