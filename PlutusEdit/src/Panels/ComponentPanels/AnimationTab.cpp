#pragma warning(disable: 4312)

#include "AnimationTab.h"

#include <ECS/Scene.h>

#include <Assets/AssetManager.h>

#include "ComponentUtil.h"

namespace Plutus
{
    void AnimationTab::DrawAnimation(Entity* ent)
    {
        mAnimation = ent->getComponent<Animation>();
        mTestures = &AssetManager::get()->mTextures;
        if (CollapseComponent<Animation>("Animation##tilemap-comp", 4))
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
                if (mTestures->mTileSets.size())
                    mCurTex = mTestures->mTileSets.begin()->first;
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

            }
            ImGui::ComboBox("##a-seq", sequences, mCurSeq, defItem);
            ImGui::PopStyleVar();

            SeqWindow();
        }
    }

    void AnimationTab::SeqWindow() {
        if (showSeqWindow) {

            auto& seq = mMode ? mAnimation->mSequences[mCurSeq] : newSeq;

            ImGui::SetNextWindowSize({ 465, 650 });
            ImGui::BeginDialog("Sequence");
            {

                if (ImGui::BeginUIGroup())
                {
                    ImGui::BeginCol("Name", 300);
                    ImGui::InputString("##seq-name", mNewSeqId);
                    ImGui::BeginCol("Time", 300);
                    ImGui::InputFloat("##seq-time", &seq.mSeqTime, 0.001f);
                    ImGui::BeginCol("Texure", 300);
                    ImGui::ComboBox("##tex-seq", mTestures->mTileSets, seq.mTexId);
                    ImGui::EndUIGroup();
                }

                ImGui::Separator();
                ImGui::Text("Frames %0.2f", ImGui::GetIO().Framerate);

                ImGui::Separator();
                auto curPos = ImGui::GetWindowSize();
                if (!seq.mTexId.empty()) {
                    auto& tex = mTestures->mTileSets[seq.mTexId];
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1, 2 });
                    if (ImGui::BeginChild("##frames-img", { curPos.x, 142 })) {
                        size_t i = 0;
                        int index2RM = -1;;
                        for (auto frame : seq.mFrames) {
                            ImGui::PushID((int)i);
                            if (i % 6 != 0)
                                ImGui::SameLine();

                            auto uv = tex.uvs[frame];
                            if (ImGui::ImageButton((ImTextureID)tex.texId, { 64, 64 }, { uv.x, uv.y }, { uv.z, uv.w })) {
                                index2RM = (int)i;
                            }
                            ImGui::PopID();
                            i++;
                        }
                        if (index2RM > -1) {
                            seq.mFrames.erase(seq.mFrames.begin() + index2RM);
                            seq.mFrame = 0;
                        }
                        ImGui::EndChild();
                    }
                    ImGui::Separator();
                    if (ImGui::BeginChild("##tex-img", { curPos.x, 217 })) {
                        for (size_t i = 0; i < tex.uvs.size(); i++) {

                            ImGui::PushID((int)i);
                            if (i % 6 != 0)
                                ImGui::SameLine();

                            auto uv = tex.uvs[i];
                            if (ImGui::ImageButton((ImTextureID)tex.texId, { 64, 64 }, { uv.x, uv.y }, { uv.z, uv.w })) {
                                seq.mFrames.push_back((int)i);
                            }
                            ImGui::PopID();
                        }
                        ImGui::EndChild();
                    }
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
                        auto uv = tex.getUV(seq.mFrames[seq.mFrame]);
                        ImGui::Image((ImTextureID)tex.texId, { width, height }, { uv.x, uv.y }, { uv.z, uv.w });
                    }
                }

                ImGui::SetCursorPos({ curPos.x * 0.5f - 50.0f, curPos.y - 34.0f });
                if (ImGui::Button("Save##s-seq") && !mNewSeqId.empty()) {
                    showSeqWindow = false;
                    if (mMode) {
                        mAnimation->swapSeq(mNewSeqId, mCurSeq);
                        mCurSeq = mNewSeqId;
                    }
                    else {
                        mAnimation->addSequence(mNewSeqId, seq);
                    }
                }}
            ImGui::EndDialog(showSeqWindow);
            if (!showSeqWindow) {
                newSeq = {};
                mNewSeqId = "";
            }
        }
    }
}