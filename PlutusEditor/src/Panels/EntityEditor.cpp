
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

    void EntityEditor::draw()
    {
        ImGui::Begin("Scene Editor");
        layers();
        entity();
        ImGui::End();
    }

    void EntityEditor::layers()
    {
        ImGui::Text(ICON_FA_LAYER_GROUP " Layers");
        ImGui::SameLine();
        static bool openLayerModal = false;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##layer"))
        {
            openLayerModal = true;
        }
        ImGui::PopStyleColor();
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

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        ImGui::Button(ICON_FA_EDIT " ##Edit_Layer_Name");
        //Remove Layer from list
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH_ALT " ##Remove_Layer"))
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
        ImGui::PopStyleColor();
        ImGui::PushItemWidth(80);
        auto name = mScene->getCurrentLayer()->name;
        if (ImGui::ComboBox<Layer>("Layers", *mLayers, name))
        {
            auto layer = mScene->setLayer(name);
            if (layer->mEntities.size() > 0)
            {
                mParentUI->setEntity(layer->mEntities[0].get());
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox(ICON_FA_EYE " ##IsVisible", &mScene->getCurrentLayer()->isVisible);
    }

    /***************************Entity List*************************/
    void EntityEditor::entity()
    {
        ImGui::Separator();
        ImGui::Text(ICON_FA_LIST_OL " Entities");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        static bool openEntModal = false;
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##ent"))
        {
            openEntModal = true;
        }
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
        ImGui::PopStyleColor();
        ImGui::Separator();
        static int selected = 0;
        static int remove = -1;
        auto layer = mScene->getCurrentLayer();
        if (ImGui::Entities("Entities##list", layer->mEntities, selected, remove))
        {
            if (layer->mEntities.size() && layer->mEntities[selected] != nullptr)
            {
                mParentUI->setEntity(layer->mEntities[selected].get());
            }
        }
        if (remove > -1)
        {
            mScene->removeEntity(layer->mEntities[remove].get());
            if (layer->mEntities.size())
            {
                mParentUI->setEntity(layer->mEntities[0].get());
            }
            else
            {
                mParentUI->setEntity(nullptr);
            }
            remove = -1;
        }
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