#include "EditorUI.h"
#include <algorithm>
#include <filesystem>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include "Serialize/SceneSerializer.h"
#include "Serialize/Serialize.h"

#include "Graphics/DebugRenderer.h"
#include "Graphics/Camera2D.h"
#include "Input/Input.h"
#include "Utils/Utils.h"

#include "IconsFontAwesome5.h"
#include "ImGuiEx.h"

#include "ECS/Scene.h"
#include <Platform/Windows/FileUtils.h>
#include <cstdio>

#define mapIn(x, min_in, max_in, min_out, max_out) (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val
#define MAX_SCALE 1000
#define MIN_SCALE 10
#define SCALE_STEP 5

namespace Plutus
{
	EditorUI *EditorUI::mInstance = nullptr;

	EditorUI::EditorUI() : mVPColor(1, 1, 1, 1), mGridColor(0)
	{
		mInput = Input::getInstance();
		if (!std::filesystem::exists("assets"))
		{
			std::filesystem::create_directories("assets/textures");
			std::filesystem::create_directories("assets/audios");
			std::filesystem::create_directories("assets/fonts");
		}

		Input::getInstance()->onFileDrop = [](const char *file)
		{
			std::printf("file: %s", file);
		};
	}

	EditorUI::~EditorUI()
	{
		saveRecents();
	}

	void EditorUI::destroy()
	{
		saveRecents();
		if (mImGui_IO)
		{
			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		mInstance = nullptr;
		mScene = nullptr;
		delete mInstance;
	}

	EditorUI *EditorUI::getInstance()
	{
		if (!mInstance)
		{
			mInstance = new EditorUI();
		}

		return mInstance;
	}

	void EditorUI::Init(Camera2D *cam)
	{

		mCamera = cam;
		mRender.init(cam);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		mImGui_IO = &ImGui::GetIO();
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

		ImGui::CaptureMouseFromApp();
		mDebugRender = Plutus::DebugRender::geInstances();
		mDebugRender->init(cam);
		//Settup font

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
		mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
		// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
		mImGui_IO->Fonts->AddFontDefault();

		mScene = CreateRef<Scene>();
		mScene->Init(mCamera);
		mComPanel.setContext(mScene);
		mEntityEditor.setContext(mScene, this);

		mFb.init(static_cast<int>(cam->getScaleScreen().x), static_cast<int>(cam->getScaleScreen().y));
		loadRecents();
	}

	void EditorUI::update(float dt)
	{
		mScene->update(dt);
	}

	void EditorUI::beginUI()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorUI::endUI()
	{
		ImGui::Render();
		int display_w, display_h;
		GLFWwindow *window = glfwGetCurrentContext();
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (mImGui_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}
	}

	void EditorUI::DrawUI()
	{
		beginUI();
		drawMainDockingWin();
		viewPort();
		viewPortControl();
		mEntityEditor.draw();
		mComPanel.drawUI(mEnt);

		if (mShowDemo)
		{
			drawDemo();
		}
		endUI();
	}

	bool ZoomViewPort(int *value, int step, int min, int max)
	{
		bool zoom = false;
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::Text("Zoom");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		if (ImGui::DragInt("##zoom-vp", value, static_cast<float>(step), min, max))
		{
			zoom = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0.0f, spacing);
		int tVal = *value;
		ImGui::PushButtonRepeat(true);
		if (ImGui::Button(ICON_FA_SEARCH_PLUS " ##zoom-vp"))
		{
			tVal += step;
			zoom = true;
		}
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button(ICON_FA_SEARCH_MINUS " ##zoom-vp"))
		{
			tVal -= step;
			zoom = true;
		}
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Reset ##zoom-vp"))
		{
			tVal = 100;
			zoom = true;
		}
		ImGui::PopButtonRepeat();
		*value = CHECKLIMIT(tVal, min, max);
		return zoom;
	}

	void EditorUI::viewPortControl()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("ViewPort Controls");
		ImGui::Columns(3, NULL, true);
		ImGui::Text("ViewPort Props");
		ImGui::PushItemWidth(150);

		if (ImGui::InputFloat("Zoom##vp", &mVPScale, 0.05f))
		{
			mVPScale = CHECKLIMIT(mVPScale, 0.20f, 6.0f);
		}

		ImGui::ColorEdit4("VP BG", glm::value_ptr(mVPColor), ImGuiColorEditFlags_AlphaBar);
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		ImGui::Text("Camera Control");
		{
			ImGui::Checkbox("Move Camera", &mMoveCam);
			ImGui::SameLine();

			if (ImGui::Button("Reset ##cam"))
			{
				mCamera->setPosition(glm::vec2(0, 0));
			}
		}
		ImGui::NextColumn();
		ImGui::Text("Grid Controls");
		ImGui::Separator();
		{
			static bool showGrid = true;
			if (ImGui::Checkbox("Enable", &showGrid))
			{
				mDebugRender->setShouldDraw(showGrid);
			}

			auto cellc = mDebugRender->getCellCount();
			int cellCount[] = {cellc.x, cellc.y};
			if (ImGui::DragInt2("Count XY", cellCount))
			{
				cellc.x = CHECKLIMIT(cellCount[0], 0, 200);
				cellc.y = CHECKLIMIT(cellCount[1], 0, 200);
				mDebugRender->setCellCount(cellc.x, cellc.y);
			}

			auto cellS = mDebugRender->getCellSize();
			int cellSize[] = {cellS.x, cellS.y};
			if (ImGui::DragInt2("Cell Size", cellSize))
			{
				cellS.x = CHECKLIMIT(cellSize[0], 0, 200);
				cellS.y = CHECKLIMIT(cellSize[1], 0, 200);
			}
			mDebugRender->setCellSize(cellS.x, cellS.y);

			float color[] = {mGridColor.x, mGridColor.y, mGridColor.z, mGridColor.w};
			if (ImGui::ColorEdit3("Grid Color", color))
			{
				mDebugRender->setColor(ColorRGBA8(static_cast<GLubyte>(color[0]) * 255, static_cast<GLubyte>(color[1]) * 255, static_cast<GLubyte>(color[2]) * 255));
				mGridColor.x = color[0];
				mGridColor.y = color[1];
				mGridColor.z = color[2];
				mGridColor.w = color[3];
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorUI::viewPort()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		static bool open = true;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::Begin("Viewport", &open, flags);
		// auto size = ImGui::GetWindowContentRegionMax();

		auto winSize = mFb.getSize();
		float aspectRation = mFb.getAspectRatio();

		auto winPos = ImGui::GetWindowContentRegionMax();

		glm::vec2 newSize(winPos.x, winPos.x / aspectRation);
		if (newSize.y > winPos.y)
		{
			newSize.y = winPos.y;
			newSize.x = winPos.y * aspectRation;
		}
		// add padding to the canvas
		newSize -= 15;
		//calculate the position centered oncanvas
		float x = max((winPos.x - newSize.x), 0) * 0.5f;
		float y = max((winPos.y - newSize.y), 0) * 0.5f;
		//set the new posotion
		ImGui::SetCursorPos({x, y});

		ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

		float xPos = mapIn(ImGui::GetIO().MousePos.x - canvas_pos.x, 0, newSize.x, 0, winSize.x);
		float yPos = mapIn(ImGui::GetIO().MousePos.y - canvas_pos.y, 0, newSize.x, 0, winSize.x);

		ImGui::Image(reinterpret_cast<void *>(mFb.getTextureId()), {newSize.x, newSize.y}, {0, 1}, {1, 0}, {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0, 0.0, 1.0});

		if (mCanvasHover = ImGui::IsItemHovered())
		{
			mouseGridCoords = mDebugRender->getSquareCoords({xPos, yPos});

			if (mInput->onKeyDown("MouseLeft"))
				mComPanel.createTiles(mouseGridCoords);

			if (mInput->onKeyPressed("r"))
				mCamera->setPosition(0, 0);

			if (mInput->onKeyPressed("z"))
				mCamera->setScale(1);

			// move the camera
			if (mInput->onKeyDown("Ctrl"))
			{
				if (mInput->onKeyPressed("MouseLeft"))
				{
					lastCoords = {xPos, yPos};
					lastCamPos = mCamera->getPosition();
				}
				if (mInput->onKeyDown("MouseLeft"))
				{
					glm::vec2 result = {xPos - lastCoords.x, yPos - lastCoords.y};
					result /= mCamera->getScale();

					mCamera->setPosition(lastCamPos - result);
				}

				auto scroll = mInput->getMouseWheel();
				if (scroll != 0)
				{
					auto newVal = mCamera->getScale() + (scroll > 0 ? 0.05f : -0.05f);
					mCamera->setScale(CHECKLIMIT(newVal, 0.20f, 6));
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}

	void EditorUI::addRecent(const std::string &path)
	{
		auto found = std::find(mRecents.begin(), mRecents.end(), path);
		if (found != mRecents.end())
		{
			mRecents.erase(found);
		}

		if (mRecents.size() > 10)
		{
			mRecents.pop_back();
		}

		mRecents.insert(mRecents.begin(), path);
	}

	void EditorUI::drawMainDockingWin()
	{
		static bool isOpen;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport *viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &isOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		drawMenu(dockspace_flags);

		ImGui::End();
	}

	void EditorUI::drawMenu(int dockspace_flags)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{
				if (ImGui::MenuItem("New", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					newScene();
				}
				if (ImGui::MenuItem("Open", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					newScene();
					mEntityEditor.loadScene("");
				}
				if (ImGui::BeginMenu("Recent"))
				{
					std::string toRemove = "";
					for (auto recent : mRecents)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
						std::string btn = ICON_FA_TRASH_ALT + std::string("##") + recent;
						if (ImGui::Button(btn.c_str()))
						{
							toRemove = recent;
						}
						ImGui::PopStyleColor();
						ImGui::SameLine();
						std::string item = ICON_FA_FILE_IMAGE + std::string(" ") + recent;
						if (ImGui::MenuItem(item.c_str()))
						{
							newScene();
							mEntityEditor.loadScene(recent);
						}
					}
					ImGui::EndMenu();
					if (!toRemove.empty())
					{
						int index = Utils::getIndex(mRecents, toRemove);
						mRecents.erase(mRecents.begin() + index);
					}
				}
				if (ImGui::MenuItem("Save", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					saveScene();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::MenuItem("Show Demo", NULL, &mShowDemo);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void EditorUI::bindFB()
	{
		mFb.bind();
		glColorMask(true, true, true, true);
		glClearColor(mVPColor.x, mVPColor.y, mVPColor.z, mVPColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
		mScene->draw();
		if (mCanvasHover)
		{
			mComPanel.render(mScene->getRenderer(), mouseGridCoords);
		}
	}

	void EditorUI::unBindFB()
	{
		mDebugRender->drawGrid();
		mFb.unBind();
	}

	void EditorUI::resizeFB(int w, int h)
	{
		// auto fbSize = mFb.getSize();
		// if (fbSize.x == w && fbSize.y != h)
		// {
		// 	mFb.resize(w, h);
		// }
	}
	void EditorUI::resizeFB(glm::vec2 size)
	{
		// resizeFB(static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void EditorUI::saveScene()
	{
		Plutus::Serializer sr;
		Plutus::sceneSerializer(sr, mScene);
		std::string filePath;
		if (Plutus::windowDialog(SAVE_FILE, filePath))
		{
			Plutus::Utils::toJsonFile(filePath, sr.getString());
			addRecent(filePath);
		}
	}

	void addColor(Plutus::Serializer *sr, const char *label, const glm::vec4 &color)
	{
		auto writer = sr->getWriter();
		writer->String(label);
		writer->StartArray();
		writer->Double(color.x);
		writer->Double(color.y);
		writer->Double(color.z);
		writer->Double(color.w);
		writer->EndArray();
	}
	void EditorUI::saveRecents()
	{
		Plutus::Serializer sr;
		auto writer = sr.getWriter();
		auto cellsCount = mDebugRender->getCellCount();
		auto cellSize = mDebugRender->getCellSize();
		writer->StartObject();
		writer->String("gridwidth");
		writer->Int(cellsCount.x);
		writer->String("gridheight");
		writer->Int(cellsCount.y);
		writer->String("CellCountX");
		writer->Int(cellSize.x);
		writer->String("CellCountY");
		writer->Int(cellSize.y);
		addColor(&sr, "vp-color", mVPColor);
		addColor(&sr, "cell-color", mGridColor);
		writer->String("recents");

		writer->StartArray();
		for (auto r : mRecents)
		{
			writer->String(r.c_str());
		}
		writer->EndArray();
		writer->EndObject();
		std::string path = std::filesystem::absolute("./pe-config.json").string();
		Plutus::Utils::toJsonFile(path, sr.getString());
	}

	void initColor(const rapidjson::Document &doc, const char *label, glm::vec4 &color)
	{
		if (doc.HasMember(label))
		{
			auto arr = doc[label].GetArray();
			color.x = static_cast<float>(arr[0].GetDouble());
			color.y = static_cast<float>(arr[1].GetDouble());
			color.z = static_cast<float>(arr[2].GetDouble());
			color.w = static_cast<float>(arr[3].GetDouble());
		}
	}

	void EditorUI::loadRecents()
	{
		rapidjson::Document doc;
		if (Utils::loadJson("./pe-config.json", &doc))
		{
			if (doc.HasMember("gridwidth"))
			{
				mDebugRender->setCellCount(doc["gridwidth"].GetInt(), doc["gridheight"].GetInt());
			}
			if (doc.HasMember("CellCountX"))
			{
				mDebugRender->setCellSize(doc["CellCountX"].GetInt(), doc["CellCountY"].GetInt());
			}
			initColor(doc, "vp-color", mVPColor);
			initColor(doc, "cell-color", mGridColor);

			if (doc["recents"].IsArray())
			{
				auto arr = doc["recents"].GetArray();
				std::cout << "aar S:" << arr.Size() << std::endl;
				for (uint32_t i = 0; i < arr.Size(); i++)
				{
					mRecents.push_back(arr[i].GetString());
					std::cout << arr[i].GetString() << std::endl;
				}

				if (mRecents.size())
				{
					mEntityEditor.loadScene(mRecents[0]);
					std::cout << "load recent" << std::endl;
				}
			}
		}
	}

	void EditorUI::newScene()
	{
		mScene->clear();
		mEnt = nullptr;
	}

} // namespace Plutus
