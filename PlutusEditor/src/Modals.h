#pragma once

#include <imgui.h>
#include "Utils/Utils.h"
#include <Platforms/Windows/FileUtils.h>
#include <string>
#include "ImGuiEx.h"
#include <Assets/AssetManager.h>

inline void TextureModal(const char* label, bool& open)
{
    if (open)
    {
        ImGui::SetNextWindowSize(ImVec2(300.0f, 270.0f));

        ImGui::OpenPopup(label);
        static char name[128];
        static std::string path = "", filename = "";
        static int columns, tileWidth, tileHeight;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        if (ImGui::BeginPopupModal(label, NULL, window_flags))
        {
            ImGui::PushItemWidth(100);
            ImGui::InputText("Name##modal", name, IM_ARRAYSIZE(name));
            ImGui::SameLine();
            static int selectedType = 0;
            ImGui::PopItemWidth();
            ImGui::Separator();
            if (ImGui::Button("Select File"))
            {
                if (Plutus::windowDialog(OPEN_FILE, path))
                {
                    filename = Plutus::Utils::getFileName(path);

                    auto ex = Plutus::Utils::getExtension(path);
                    if (ex.compare("png") != 0)
                    {
                        path = "";
                    }
                }
            }
            ImGui::SameLine();
            ImGui::Text(filename.c_str());
            ImGui::Separator();
            ImGui::PushItemWidth(100);
            ImGui::InputInt("Columns", &columns);
            if (columns)
            {
                ImGui::InputInt("Tile Width", &tileWidth);
                ImGui::InputInt("Tile Height", &tileHeight);
            }
            ImGui::PopItemWidth();
            ImGui::Separator();
            ImGui::SetCursorPos({ 195.0f, 235.0f });
            if (ImGui::Button("save##modal"))
            {
                Plutus::AssetManager::get()->mTextures.addTexture(name, path, columns, tileWidth, tileWidth);
                std::string texName(name);
                if (!texName.empty())
                {
                    open = false;
                    filename = "";
                    path = "";
                    selectedType = 0;
                    columns = 0;
                    tileWidth = 0;
                    tileWidth = 0;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Close##modal"))
                open = false;

            ImGui::EndPopup();
        }
    }
}