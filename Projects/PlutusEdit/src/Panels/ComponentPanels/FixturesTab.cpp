#include "FixturesTabs.h"

#include <vector>
#include <Log/Logger.h>

extern std::vector<std::string> fixturesType = {
    "Edge", "Circle","Box"
};

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

        ImGui::Row("friction", textWidth);
        ImGui::DragFloat("##fix-friction", &fix.friction, 0.01f, 0.01f, 5, "%0.2f");
        if (btype) {
            ImGui::Row("density", textWidth);
            ImGui::DragFloat("##fix-density", &fix.density, 0.1f, 0, 20, "%0.02f");
            ImGui::Row("restitution", textWidth);
            ImGui::DragFloat("##fix-restitution", &fix.restitution, 0.00f, 0.01f, 1, "%0.2f");
            ImGui::Row("isSensor", textWidth);
            ImGui::Checkbox("Is Sensor", &fix.isSensor);
        }
    }
    ImGui::PopID();

    return remove;
}

void drawFixtures(Plutus::PhysicBodyComponent* physicBody) {
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
        for (auto& fix : physicBody->mFixtures) {
            bool cp = false;
            if (drawFixture(fix, id++, physicBody->mBodyType, cp)) {
                removeId = id - 1;
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
}