#include "AssetsTab.h"

#include <memory>
#include <imgui.h>
#include <filesystem>

#include <Utils/FileIO.h>
#include <Assets/temp/Assets.h>

#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"
#include <Assets/SoundEngine.h>
#include <Utils/FileIO.h>

#include "../Config.h"


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
        if (ImGui::TransparentButton(state ? ICON_FA_PLAY : ICON_FA_STOP, true, { 0,0,1,1 })) {
            state ? SoundEngine.playEvent(event) : SoundEngine.stopEvent(event);
        }
        ImGui::SameLine();
        ImGui::Text(name);
    }

    void AssetsTab::drawFilter()
    {
        ImGui::BeginUIGroup(ImGuiTableFlags_SizingStretchProp);
        ImGui::BeginCol("Filter");
        ImGui::InputString("##a-filter", filter);
        ImGui::EndUIGroup();
        ImGui::Separator();
    }

    void AssetsTab::drawAssets()
    {
        if (ImGui::Begin("Assets")) {
            auto mAsset = AssetManager2::get();
            if (std::filesystem::exists("./assets/")) {
                if (ImGui::BeginTabBar("##TabBar"))
                {

                    if (ImGui::BeginTabItem("Disk Assets"))
                    {
                        drawFilter();
                        if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                            drawDiskAssets("./assets/");
                            ImGui::EndChild();
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Scene Assets"))
                    {
                        drawFilter();
                        if (ImGui::BeginChild("##assets-files", { 0,0 }, false)) {
                            int id = 0;

                            drawTreeNode("Fonts", mAsset->getAssets<Font>(), id);
                            drawTreeNode("Textures", mAsset->getAssets<Texture2>(), id);
                            drawTreeNode("Sounds", mAsset->getAssets<Sound>(), id);
                            // drawTreeNode("Scenes", Config::get().mProjects[Config::get().OpenProject], id);
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
        ImGui::BeginDialog("Asset Modal");
        ImGui::Text(assetType.id.c_str());
        ImGui::Separator();
        if (assetType.type.compare("Textures") == 0) {
            auto texture = AssetManager2::get()->getAsset<Texture2>(assetType.id);
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

    void AssetsTab::drawDiskAssets(std::string path)
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
                            Config::get().mProject->add(path.c_str());
                        }

                        if (!isScene && ImGui::MenuItem(("Add " + name + " to scene").c_str())) {
                            std::replace(path.begin(), path.end(), '\\', '/');
                            selectedDir = path;
                        }
                        ImGui::EndPopup();
                    }
                }
            }
            id++;
        }
    }

    void AssetsTab::processFile()
    {
        static char name[128];
        static int fontSize = 20;
        ImVec2 vec = { 1,1 };

        int type = 0;

        if (!selectedDir.empty()) {
            auto ex = Utils::getExtension(selectedDir);

            size_t start = 9;
            size_t end = selectedDir.find_first_of("/", start);
            auto substr = selectedDir.substr(start, end - start);

            bool show = true;

            ImGui::BeginDialog("Asset Modal");
            //Texure Or Font Id
            ImGui::Text("Asset Id");
            ImGui::SameLine();
            ImGui::InputText("##asset-modal", name, IM_ARRAYSIZE(name));
            ImGui::Separator();

            if (substr.compare("textures") == 0 && imgTypes[ex]) {

                if (texture.mTexId == 0) {
                    texture = Texture2(selectedDir, texture.mColumns, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                }
                type = 1;
                showTexure(texture, true);
            }

            if (substr.compare("fonts") == 0 && fontTypes[ex]) {
                ImGui::InputInt("Font Size", &fontSize);
                type = 2;
            }

            if (substr.compare("sounds") == 0 && soundsTypes[ex]) {
                type = 3;
                if (!aEvent) {
                    aEvent = SoundEngine.createEvent("temp", selectedDir, 0);
                }
                audioPlay(aEvent, Utils::getFileName(selectedDir).c_str());
            }

            ImGui::Separator();
            if (ImGui::Button("save##modal") && strlen(name) > 0)
            {
                switch (type) {
                case 1:
                    AssetManager2::get()->addAsset<Texture2>(name, selectedDir, texture.mColumns, texture.mTileWidth, texture.mTileHeight, texfilter.filter, texfilter.filter);
                    break;
                case 2:
                    AssetManager2::get()->addAsset<Font>(name, selectedDir, fontSize);
                    break;
                case 3:
                    SoundEngine.add(name, selectedDir, 0);
                    break;
                }
                show = false;
            }
            ImGui::EndDialog(show);

            if (!show) {
                selectedDir = "";
                memset(name, 0, 128);
                fontSize = 20;
                if (texture.mWidth) {
                    glDeleteTextures(1, &texture.mTexId);
                }
                texture = Texture2("");
                if (aEvent) {
                    delete aEvent;
                    aEvent = nullptr;
                }
            }
        }

    }

    void AssetsTab::showTexure(Texture2& texture, bool newTex) {

        static float scale = 1.0f;
        if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingStretchProp)) {
            ImGui::BeginCol("Columns");
            if (ImGui::InputInt("##Columns", &texture.mColumns)) texture.calculateUV();

            ImGui::BeginCol("Tile Width");
            if (ImGui::InputInt("##Tile Width", &texture.mTileWidth))  texture.calculateUV();

            ImGui::BeginCol("Tile Height");
            if (ImGui::InputInt("##Tile Height", &texture.mTileHeight)) texture.calculateUV();
            if (newTex) {
                ImGui::BeginCol("Filters");
                if (ImGui::BeginCombo("##Filters", texfilter.Name))
                {
                    for (int i = 0; i < IM_ARRAYSIZE(filters); i++) {
                        bool is_selected = filters[i].filter == texfilter.filter;
                        if (ImGui::Selectable(filters[i].Name, is_selected)) {
                            texfilter = filters[i];

                            if (texture.mWidth) {
                                texture.mWidth = 0;
                                glDeleteTextures(1, &texture.mTexId);
                            }

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::BeginCol("Scale");
            ImGui::DragFloat("##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
            ImGui::EndUIGroup();
        }
        std::vector<vec3i> selected;
        ImGui::DrawTexture(&texture, 400, 300, scale, &selected);
    }
}