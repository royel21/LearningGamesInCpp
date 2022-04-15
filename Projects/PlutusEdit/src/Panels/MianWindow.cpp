#include "MianWindow.h"

#include <glm/glm.hpp>

#include <string>

#include <Utils/FileIO.h>
#include <Utils/Utils.h>

#include "../Config.h"
#include "../Helpers/ImGuiStyle.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/ImGuiDialog.h"
#include "../Helpers/IconsFontAwesome5.h"

#include <Platforms/Windows/FileUtils.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../App.h"

namespace Plutus
{
    MianWindow::~MianWindow()
    {
        if (isInitialized) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    }

    void MianWindow::init(Config* config, App* app)
    {
        mConfig = config;
        mApp = app;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto mImGui_IO = &ImGui::GetIO();
        mImGui_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
        mImGui_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
        ImGui_ImplOpenGL3_Init("#version 150");
        ImGui::CaptureMouseFromApp();
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        const char* openSanf = "assets/fonts/OpenSans/OpenSans-Regular.ttf";
        const char* fontawson = "assets/fonts/fa-solid-900.ttf";
        if (exists(openSanf) && exists(fontawson)) {
            mImGui_IO->Fonts->AddFontFromFileTTF(openSanf, 20.0f);
            mImGui_IO->Fonts->AddFontFromFileTTF(fontawson, 18.0f, &icons_config, icons_ranges);
            // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
            mImGui_IO->Fonts->AddFontDefault();
        }
        isInitialized = true;
    }

    void MianWindow::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        DoStyle();
        DockingWindow();
    }

    void MianWindow::End()
    {
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            auto contextBackup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(contextBackup);
        }

        auto& projects = mConfig->mProjects;
        if (!mProjToRemove.empty())
        {
            if (mConfig->mProjects.size() > 1) {
                mConfig->mProjects.erase(mProjToRemove);

                if (mConfig->mProjects.size()) {
                    mConfig->LoadProject(mConfig->mProjects.begin()->first);
                }
            }
        }
        mProjToRemove = "";
    }

    void MianWindow::DockingWindow() {
        static bool isOpen;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::PopStyleVar(2);
        // // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        if (ImGui::Begin("DockSpace Demo", &isOpen, window_flags)) {
            ImGui::PopStyleVar();


            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            //Draw Menu
            MenuGui(dockspace_flags);

        }
        ImGui::End();
    }

    void MianWindow::MenuGui(int flags) {
        bool noSplit = (flags & ImGuiDockNodeFlags_NoSplit) != 0;
        static bool open = false;
        static bool edit = false;
        static std::string projName;
        auto& projects = mConfig->mProjects;
        /**************************************** Draw Editor Menu *******************************************************/
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Files"))
            {
                if (ImGui::MenuItem(ICON_FA_FILE " New", "Ctrl+N", noSplit))
                {
                    open = true;
                }

                if (ImGui::BeginMenu(ICON_FA_LIST " Projects"))
                {
                    for (auto& recent : projects)
                    {
                        std::string btn = ICON_FA_TRASH_ALT + std::string("##") + recent.first;
                        if (ImGui::TransparentButton(btn.c_str())) {
                            mProjToRemove = recent.first;
                        }
                        ImGui::SameLine();
                        std::string btnEdit = ICON_FA_EDIT + std::string("##") + recent.first;
                        if (ImGui::TransparentButton(btnEdit.c_str())) {
                            projName = toEdit = recent.first;
                            edit = true;
                        }
                        ImGui::SameLine();
                        std::string item = ICON_FA_FILE_IMAGE + std::string(" ") + recent.first;
                        if (ImGui::MenuItem(item.c_str()))
                        {
                            mConfig->LoadProject(recent.first.c_str());
                            mConfig->OpenProject = recent.first;
                        }
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem(ICON_FA_SIGN_OUT_ALT " Exit", "Ctrl+X", noSplit))
                {
                    mExit = true;
                }
                ImGui::EndMenu();
            }
            static bool mShowDemo;
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Show Demo", NULL, &mShowDemo);
                static bool isAlwaysOnTop = false;
                ImGui::MenuItem("Always On Top", NULL, &isAlwaysOnTop);
                mApp->setAlwaysOnTop(isAlwaysOnTop);

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
            if (mShowDemo) {
                ImGui::ShowDemoWindow();
            }
        }

        if (open) ImGui::NewFileDialig("New Project", [&](const std::string& name) {
            if (!name.empty()) mConfig->CreateProj(name.c_str());
            open = false;
            });

        if (edit && ImGui::BeginDialogText("Edit Project")) {
            ImGui::BeginUIGroup();
            ImGui::BeginCol("Name", 200);
            ImGui::InputString("##n-p", toEdit);
            ImGui::EndUIGroup();
            ImGui::EndDialogText([&](bool save) {
                if (save) {
                    mConfig->RenameProj(projName, toEdit);
                    toEdit = "";
                    projName = "";
                }
                edit = false;
                });
        }
    }
}