#include "FixturesTabs.h"

#include <vector>
#include <Log/Logger.h>

#include <Input/Input.h>
#include <Physics/PUtils.h>

namespace Plutus
{
    Fixture* mFixture = nullptr;
    Vec2f startPos, startOffset;

    std::vector<std::string> fixturesType = {
        "Edge", "Circle","Box"
    };

    void bitmask(const char* id, uint16_t& val) {
        ImGui::PushID(id);
        ImGui::BeginGroup();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1.0f, 1.0f });
        for (uint32_t i = 1; i < 0xffff; i *= 2) {

            ImGui::PushStyleColor(ImGuiCol_Button, val & i ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0, 0, 0, 1 });

            if (ImGui::Button((id + std::to_string(i)).c_str(), { 15,15 })) {
                val = val & i ? val - i : val + i;
            }

            if (i != 0x80) {
                ImGui::SameLine();
            }
            ImGui::PopStyleColor();
        }
        ImGui::PopStyleVar();
        ImGui::EndGroup();
        ImGui::PopID();
    }

    bool CollapseTab(const char* label, int id, bool& remove, bool& copy) {
        bool isOpen = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_AllowItemOverlap);

        ImGui::PushID(id);
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 60);
        if (ImGui::TransparentButton(ICON_FA_CLIPBOARD, false, { 0,1,0,1 })) {
            copy = true;
        }
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 25);
        if (ImGui::TransparentButton(ICON_FA_TRASH_ALT, false, { 1,0,0,1 })) {
            isOpen = false;
            remove = true;
        }
        ImGui::PopID();
        return isOpen;
    }

    bool drawFixture(Plutus::Fixture& fix, int index, int btype, bool& copy) {
        auto name = fixturesType[fix.type] + "##id-" + std::to_string(index);
        bool remove = false;
        ImGui::PushID(name.c_str());
        if (CollapseTab(name.c_str(), index, remove, copy)) {
            float textWidth = ImGui::GetContentRegionAvailWidth() * 0.3f;
            ImGui::Row("Offset", textWidth);
            ImGui::Draw2Float("##fix-offset", fix.offset);

            if (fix.type == 1) {
                ImGui::Row("Radius", textWidth);
                ImGui::DragFloat("##fix-radius", &fix.radius, 1, 0.01f, 2000, "%0.2f");
            }
            else {
                ImGui::Row("Size", textWidth);
                ImGui::Draw2Float("##fix-size", fix.size, 1, "W", "H");
            }

            ImGui::Row("Friction", textWidth);
            ImGui::DragFloat("##fix-friction", &fix.friction, 0.01f, 0.01f, 5, "%0.2f");
            if (btype) {
                ImGui::Row("Density", textWidth);
                ImGui::DragFloat("##fix-density", &fix.density, 0.1f, 0, 20, "%0.02f");
                ImGui::Row("Restitution", textWidth);
                ImGui::DragFloat("##fix-restitution", &fix.restitution, 0.00f, 0.01f, 1, "%0.2f");
                ImGui::Row("IsSensor", textWidth);
                ImGui::Checkbox("Is Sensor", &fix.isSensor);
            }
            ImGui::Row("Category", textWidth);
            bitmask(("##cat-" + std::to_string(index)).c_str(), fix.category);
            ImGui::Row("GoupId", textWidth);
            bitmask(("##gpid-" + std::to_string(index)).c_str(), fix.group);
            ImGui::Row("Mask", textWidth);
            bitmask(("##mask-" + std::to_string(index)).c_str(), fix.mask);
        }
        ImGui::PopID();

        return remove;
    }

    void drawFixtures(Plutus::PhysicBodyComponent* physicBody, Config* config) {
        ImGui::Separator();
        if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " Add Fixture##cp")) {
            ImGui::OpenPopup("AddFixture");
        }
        ImGui::Separator();
        if (ImGui::BeginPopup("AddFixture"))
        {
            ImGui::Text("Fixtures");
            ImGui::Separator();
            int type = 0;
            for (auto& fix : fixturesType) {
                if (ImGui::MenuItem(fix.c_str())) {
                    physicBody->mFixtures.push_back({ type });
                }
                type++;
            }

            ImGui::EndPopup();
        }

        int id = 0;
        int removeId = -1;
        int copyId = -1;
        if (ImGui::BeginChild("fixture-list", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar)) {
            auto pos = config->mRender.mCamera->convertScreenToWold(config->mMouseCoords);
            auto epos = config->mProject.mEnt.getPosition();

            for (auto& fix : physicBody->mFixtures) {
                bool cp = false;
                if (config->isHover && Input.onKeyPressed("MouseLeft")) {
                    Vec4f rect;
                    if (fix.type == 1) {
                        auto c = Circle2d((fix.offset + epos), fix.radius);
                        if (PUtils::PointInCircle(pos, &c)) {
                            mFixture = &fix;
                            ImGui::SetNextItemOpen(true);
                        }
                    }
                    else {
                        auto box = Box2d(fix.offset + epos, fix.size);
                        if (PUtils::PointInBox(pos, &box)) {
                            mFixture = &fix;
                            ImGui::SetNextItemOpen(true);
                        }
                    }
                }

                if (drawFixture(fix, id++, physicBody->mBodyType, cp)) {
                    removeId = id - 1;
                    if (mFixture == &fix) mFixture = nullptr;
                }

                if (cp) copyId = id;

            }
        }
        ImGui::EndChild();

        if (removeId > -1) {
            physicBody->mFixtures.erase(physicBody->mFixtures.begin() + removeId);
        }

        if (copyId > -1) {
            physicBody->mFixtures.emplace_back(physicBody->mFixtures[copyId - 1]);
        }

        if (mFixture != nullptr && !Input.isCtrl) {
            if (Input.onKeyPressed("MouseLeft")) {
                startPos = config->mMouseCoords;
                startOffset = mFixture->offset;
            }

            if (Input.onKeyDown("MouseLeft")) {
                auto offset = (config->mMouseCoords - startPos) / config->mRender.mCamera->getScale();;
                mFixture->offset = startOffset + offset;
            }
        }
    }

}