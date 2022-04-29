#pragma warning(disable: 4312)

#include "AnimationTab.h"

#include <ECS/Scene.h>

#include "ComponentUtil.h"
#include <Assets/Assets.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Plutus
{
    void AnimationTab::draw(Config* config)
    {
        mAnimation = config->mProject.mEnt.getComponent<AnimationComponent>();
        auto& mTextures = AssetManager::get()->getAssets<Texture>();
        if (CollapseComponent<AnimationComponent>("Animation##tilemap-comp", 3, config))
        {
            auto& sequences = mAnimation->mSequences;
            auto it = sequences.begin();
            std::string defItem = it != sequences.end() ? it->first : "";

            ImGui::Text("Sequences");
            ImGui::Separator();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 4 });
            if (ImGui::TransparentButton(ICON_FA_PLUS_SQUARE "##add-seq", true)) {
                showSeqWindow = true;
                mNewSeqId = "";
                mMode = NEW;
                if (mTextures.size())
                    mCurTex = mTextures.begin()->first;
            }
            if (sequences.size() && !mCurSeq.empty()) {
                if (ImGui::TransparentButton(ICON_FA_EDIT "##edit-seq", true)) {
                    showSeqWindow = true;
                    mCurrentSeq = &sequences[mCurSeq];
                    mNewSeqId = mCurSeq;
                    mCurTex = mCurrentSeq->mTexId;
                    mMode = EDIT;
                }
            }
            if (ImGui::TransparentButton(ICON_FA_TRASH "##rm-seq", true, { 1,0,0,1 })) {
                sequences.erase(mCurSeq);
                if (sequences.size()) {
                    mCurrentSeq = &sequences.begin()->second;
                    mCurSeq = sequences.begin()->first;
                }
                else {
                    mCurrentSeq = nullptr;
                    mCurSeq = "";
                }
            }


            ImGui::ComboBox("##a-seq", sequences, mCurSeq, defItem);

            auto found = sequences.find(mCurSeq);
            if (found != sequences.end()) {
                auto& seq = found->second;
                auto tex = AssetManager::get()->getAsset<Texture>(seq.mTexId);

                if (tex) {
                    ImGui::Row("Time");
                    ImGui::InputFloat("##seq-time", &seq.mSeqTime, 0.001f);
                    ImGui::Separator();

                    ImGui::BeginChild("Anim", { 0, 224 });
                    {
                        float tileSize = 128;

                        float center = ImGui::GetContentRegionAvailWidth() * 0.5f - tileSize / 2.0f;
                        ImGui::SetCursorPos({ center, tileSize / 2.0f });

                        if (seq.mFrames.size()) {
                            time += (1000.0f / ImGui::GetIO().Framerate) / 1000.0f;
                            if (time > seq.mSeqTime) {
                                seq.mFrame = ++seq.mFrame % seq.mFrames.size();
                                time = 0;
                            }
                            auto uv = tex->getUV(seq.mFrames[seq.mFrame]);
                            ImGui::Image((ImTextureID)tex->mTexId, { tileSize, tileSize }, { uv.x, uv.y }, { uv.z, uv.w });
                        }
                    }
                    ImGui::EndChild();
                }
            }
            ImGui::PopStyleVar();

            SeqWindow();
        }
    }

    void AnimationTab::SeqWindow() {
        if (showSeqWindow) {
            auto& mTextures = AssetManager::get()->getAssets<Texture>();
            auto found = mAnimation->mSequences.find(mCurSeq);
            auto& seq = mMode == EDIT ? found->second : newSeq;

            float width = ImGui::GetContentRegionAvailWidth() * .3f;

            ImGui::SetNextWindowSize({ 465, 650 });
            ImGui::BeginDialog("Sequence");
            {
                {
                    ImGui::Row("Name", width);
                    ImGui::InputText("##seq-name", &mNewSeqId);
                    ImGui::Row("Time", width);
                    ImGui::InputFloat("##seq-time", &seq.mSeqTime, 0.001f);
                    ImGui::Row("Texure", width);
                    ImGui::ComboBox("##tex-seq", mTextures, seq.mTexId);
                }

                ImGui::Separator();
                ImGui::Text("All Frames - FPS:%0.2f", ImGui::GetIO().Framerate);
                ImGui::Separator();

                auto curPos = ImGui::GetWindowSize();
                auto tex = AssetManager::get()->getAsset<Texture>(seq.mTexId);
                if (tex) {
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1, 2 });
                    if (ImGui::BeginChild("##tex-img", { curPos.x, 217 })) {
                        for (size_t i = 0; i < tex->uvs.size(); i++) {

                            ImGui::PushID((int)i);
                            if (i % 6 != 0)
                                ImGui::SameLine();

                            auto uv = tex->uvs[i];
                            if (ImGui::ImageButton((ImTextureID)tex->mTexId, { 64, 64 }, { uv.x, uv.y }, { uv.z, uv.w })) {
                                seq.mFrames.push_back((int)i);
                            }
                            ImGui::PopID();
                        }
                    }
                    ImGui::EndChild();
                    ImGui::Separator();
                    ImGui::Text("Frames");
                    ImGui::Separator();
                    if (ImGui::BeginChild("##frames-img", { curPos.x, 142 })) {
                        size_t i = 0;
                        int index2RM = -1;;
                        for (auto frame : seq.mFrames) {
                            ImGui::PushID((int)i);
                            if (i % 6 != 0)
                                ImGui::SameLine();

                            auto uv = tex->uvs[frame];
                            if (ImGui::ImageButton((ImTextureID)tex->mTexId, { 64, 64 }, { uv.x, uv.y }, { uv.z, uv.w })) {
                                index2RM = (int)i;
                            }
                            ImGui::PopID();
                            i++;
                        }
                        if (index2RM > -1) {
                            seq.mFrames.erase(seq.mFrames.begin() + index2RM);
                            seq.mFrame = 0;
                        }
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();

                    ImGui::Separator();
                    static float width = 64.0f;
                    static float height = 64.0f;

                    ImGui::BeginTable("test", 2);
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Width");
                        ImGui::TableNextColumn();

                        ImGui::SetNextItemWidth(150);
                        ImGui::InputFloat("##spr-w", &width, 1.0f);
                        ImGui::TableNextColumn();
                        ImGui::Text("Height");
                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(150);
                        ImGui::InputFloat("##spr-h", &height, 1.0f);
                    }
                    ImGui::EndTable();
                    ImGui::SameLine();

                    ImGui::SetCursorPos({ curPos.x * 0.7f - width / 2, curPos.y - height - 50.0f });
                    if (seq.mFrames.size()) {
                        time += (1000.0f / ImGui::GetIO().Framerate) / 1000.0f;
                        if (time > seq.mSeqTime) {
                            seq.mFrame = ++seq.mFrame % seq.mFrames.size();
                            time = 0;
                        }
                        auto uv = tex->getUV(seq.mFrames[seq.mFrame]);
                        ImGui::Image((ImTextureID)tex->mTexId, { width, height }, { uv.x, uv.y }, { uv.z, uv.w });
                    }
                }

                ImGui::EndDialog(showSeqWindow, [&](bool save) {

                    if (save && !mNewSeqId.empty()) {
                        if (!mNewSeqId.empty()) {
                            if (mMode == EDIT) {
                                if (mNewSeqId.compare(mCurSeq) != 0) {
                                    mAnimation->updateSeq(mCurSeq, mNewSeqId);
                                    mCurSeq = mNewSeqId;
                                }
                            }
                            else {
                                mAnimation->addSequence(mNewSeqId, seq);
                                mNewSeqId = "";
                                newSeq.mFrames = {};
                                newSeq.mFrame = 0;
                            }
                        }
                        showSeqWindow = false;
                    }
                    newSeq = {};
                    mNewSeqId = "";

                    });
            }
        }
    }
}