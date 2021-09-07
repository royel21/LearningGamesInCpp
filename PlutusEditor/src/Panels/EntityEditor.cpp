
#include "EntityEditor.h"
#include "ImGuiEx.h"
#include "Input/Input.h"
#include "Assets/AssetManager.h"
#include "ECS/Scene.h"
#include "IconsFontAwesome5.h"
#include <Serialize/SceneLoader.h>
#include "Utils/Utils.h"
#include "EditorUI.h"
#include <Platform/Windows/FileUtils.h>
#include "Modals.h"

namespace Plutus
{
    void EntityEditor::setContext(const Ref<Scene>& context, EditorUI* parent)
    {
        mParentUI = parent;
        mInput = Plutus::Input::getInstance();
        mScene = context;
        mLayers = context->getLayers();
    }

    void EntityEditor::drawTileset()
    {
    }

    void EntityEditor::removeLayer()
    {
        if (mScene->removeLayer(mScene->getCurrentLayer()->name))
        {
            mLayers = mScene->getLayers();
            if (mLayers->size())
            {
                auto it = mLayers->begin();
                auto layer = mScene->setLayer(it->first);
                if (layer->mEntities.size())
                {
                    mParentUI->setEntity(layer->mEntities[0].get());
                }
            }
            else
            {
                mParentUI->setEntity(nullptr);
            }
        }
    }

    void EntityEditor::draw()
    {
        bool open = true;
        ImGui::Begin("Scene Editor", &open, ImGuiWindowFlags_NoScrollbar);
        layers();
        entity();
        ImGui::End();
    }

    void EntityEditor::layers()
    {
        auto size = ImGui::GetWindowContentRegionMax();
        ImGui::Text(ICON_FA_LAYER_GROUP " Layers");
        ImGui::Separator();
        static bool openLayerModal = false;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##layer"))
            openLayerModal = true;
        ImGui::SameLine();
        ImGui::Button(ICON_FA_EDIT " ##Edit_Layer_Name");
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH_ALT " ##Remove_Layer"))
            removeLayer();
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::Checkbox(ICON_FA_EYE " ##IsVisible", &mScene->getCurrentLayer()->isVisible);
        ImGui::PushItemWidth(size.x);
        auto name = mScene->getCurrentLayer()->name;
        if (ImGui::ComboBox<Layer>("##Layers", *mLayers, name))
        {
            auto layer = mScene->setLayer(name);
            if (layer->mEntities.size() > 0)
            {
                mParentUI->setEntity(layer->mEntities[0].get());
            }
        }
        ImGui::PopItemWidth();
        if (openLayerModal)
        {
            auto pos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(ImVec2(pos.x + 100, pos.y + 60));
            std::string newLayer = LayerModal("New Layer", &openLayerModal);
            if (!openLayerModal && !newLayer.empty())
            {
                mScene->addLayer(newLayer);
                mParentUI->setEntity(nullptr);
            }
        }
    }

    void EntityEditor::entityList()
    {
        static int selected = 0;
        auto layer = mScene->getCurrentLayer();
        bool isSelected = false;
        auto entities = layer->mEntities;
        int remove = -1;
        for (uint32_t i = 0; i < entities.size(); i++)
        {
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
            if (ImGui::Button(ICON_FA_TRASH_ALT " ##remove"))
                remove = i;
            ImGui::PopID();
            ImGui::PopStyleColor();
            ImGui::SameLine();
            if (ImGui::Selectable(entities[i]->getName().c_str(), i == selected))
            {
                selected = i;
                mParentUI->setEntity(layer->mEntities[i].get());
            }

            if (remove != -1)
            {
                mScene->removeEntity(entities[i].get());
                mParentUI->setEntity(layer->mEntities.size() ? layer->mEntities[0].get() : nullptr);
            }
        }
    }
    /***************************Entity List*************************/
    void EntityEditor::entity()
    {
        auto size = ImGui::GetWindowContentRegionMax();
        static bool openEntModal = false;
        static int remove = -1;
        auto layer = mScene->getCurrentLayer();

        ImGui::Separator();
        ImGui::Text(ICON_FA_LIST_OL " Entities");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##ent"))
            openEntModal = true;
        ImGui::PopStyleColor();
        ImGui::Separator();
        ImGui::BeginChild("ent-list", { size.x, size.y * 0.35f });
        entityList();
        ImGui::EndChild();
        if (openEntModal)
        {
            auto pos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(ImVec2(pos.x + 100, pos.y + 120));
            std::string newEntity = LayerModal("New Entity", &openEntModal);
            if (!openEntModal && !newEntity.empty())
            {
                mParentUI->setEntity(mScene->createEntity(newEntity));
            }
        }
        assetsList();
    }

    void EntityEditor::assetsList()
    {
        auto size = ImGui::GetWindowContentRegionMax();
        static bool open = false;

        ImGui::Separator();
        ImGui::Text("Assets List");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##asset"))
            open = true;
        ImGui::Separator();
        ImGui::BeginChild("assets-list", { 0, 0 });
        std::string toRemove;
        for (auto& tex : AssetManager::get()->mTextures.mTileSets)
        {

            if (ImGui::Button((ICON_FA_TRASH_ALT + std::string(" ##") + tex.first).c_str()))
                toRemove = tex.first;

            ImGui::SameLine();
            ImGui::Text((tex.first).c_str());
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();
        if (!toRemove.empty())
            AssetManager::get()->mTextures.removeTexture(toRemove);

        TextureModal("Add texture##asset", open);
    }

    std::string EntityEditor::LayerModal(char* label, bool* open)
    {
        std::string result = "";
        if (*open)
        {
            ImGui::SetNextWindowSize(ImVec2(260.0f, 95.0f));

            ImGui::OpenPopup(label);
            static char newlayer[128];
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
            if (ImGui::BeginPopupModal(label, NULL, window_flags))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
                ImGui::InputText("Name##modal-1 ", newlayer, IM_ARRAYSIZE(newlayer));

                if (ImGui::Button("Save##modal-1") && !std::string(newlayer).empty())
                {
                    result = newlayer;
                    *open = false;
                }

                ImGui::SameLine();
                if (ImGui::Button("Close##modal-1"))
                    *open = false;

                ImGui::PopStyleColor(1);
                ImGui::EndPopup();
            }
        }
        return result;
    }

    void EntityEditor::loadScene(std::string path)
    {
        if (!path.empty() || Plutus::windowDialog(OPEN_FILE, path))
        {
            Plutus::SceneLoader::loadFromJson(path.c_str(), mScene);
            auto layer = mScene->getCurrentLayer();

            if (layer->mEntities.size())
            {
                mParentUI->setEntity(layer->mEntities[0].get());
                mParentUI->addRecent(path);
            }
        }
    }
} // namespace Plutus