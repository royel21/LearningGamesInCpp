#include "EditorUI.h"
#include <algorithm>
#include <filesystem>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <Serialize/SceneSerializer.h>
#include <Serialize/Serialize.h>

#include <Graphics/DebugRenderer.h>
#include <Graphics/Camera2D.h>
#include <Graphics/GLSL.h>
#include <Input/Input.h>
#include <Utils/Utils.h>

#include "IconsFontAwesome5.h"
#include "ImGuiEx.h"

#include <Time/Timer.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Serialize/SceneLoader.h>
#include <Platforms/Windows/FileUtils.h>

#include <cstdio>
#include "TestWin.h"


#define mapIn(x, min_in, max_in, min_out, max_out) (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val
#define MAX_SCALE 1000
#define MIN_SCALE 10
#define SCALE_STEP 5

namespace Plutus
{
	EditorUI* EditorUI::mInstance = nullptr;

	EditorUI::EditorUI() : mVPColor(1, 1, 1, 1), mGridColor(0)
	{
		mInput = Input::getInstance();
		mInput->addEventListener(this);
		if (!std::filesystem::exists("assets"))
		{
			std::filesystem::create_directories("assets/textures");
			std::filesystem::create_directories("assets/audios");
			std::filesystem::create_directories("assets/fonts");
		}

		Input::getInstance()->onFileDrop = [](const char* file)
		{
			std::printf("file: %s", file);
		};
	}

	EditorUI::~EditorUI()
	{
		saveRecents();
		mInput->addRemoveListener(this);
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

	EditorUI* EditorUI::getInstance()
	{
		if (!mInstance)
		{
			mInstance = new EditorUI();
		}

		return mInstance;
	}

	void EditorUI::Init(Camera2D* cam)
	{
		mCamera = cam;
		mRender.init();
		mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		mImGui_IO = &ImGui::GetIO();
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
		ImGui_ImplOpenGL3_Init("#version 150");

		ImGui::CaptureMouseFromApp();
		mDebugRender = Plutus::DebugRender::geInstances();
		mDebugRender->init(mCamera);
		//Settup font

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
		mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
		// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
		mImGui_IO->Fonts->AddFontDefault();

		mScene = CreateRef<Scene>();
		// mComPanel.setContext(mScene);
		mEntityEditor.setContext(mScene, this);

		auto size = cam->getScaleScreen();
		mFb.init(static_cast<int>(size.x), static_cast<int>(size.y));
		mPicker.init(static_cast<int>(size.x), static_cast<int>(size.y), true);
		loadRecents();
	}

	void EditorUI::update(float dt)
	{
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
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (mImGui_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto contextBackup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(contextBackup);
		}
	}

	void EditorUI::drawUI()
	{
		beginUI();
		drawMainDockingWin();
		mEntityEditor.draw();
		mAssetsTab.drawAssets();
		viewPort();
		viewPortControl();

		if (mShowDemo)
		{
			drawDemo();
		}
		ImGui::drawTest();
		endUI();

		bindFB();
		unBindFB();
	}

	bool ZoomViewPort(int* value, int step, int min, int max)
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

		ImGui::ColorEdit4("VP BG", glm::value_ptr(mVPColor), ImGuiColorEditFlags_AlphaBar);
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		ImGui::Text("Camera Control");
		{
			ImGui::PushItemWidth(113);
			mVPScale = mCamera->getScale();
			if (ImGui::InputFloat("Zoom##vp", &mVPScale, 0.05f))
			{
				mCamera->setScale(CHECKLIMIT(mVPScale, 0.20f, 6.0f));
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Reset##cam")) mCamera->setScale(1.0f);

			ImGui::PushItemWidth(120);
			auto pos = mCamera->getPosition();
			if (ImGui::DragFloat2("Position##cam", glm::value_ptr(pos), 1, 0, 0, "%.0f")) {
				mCamera->setPosition(pos);
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Reset##campos")) mCamera->setPosition({ 0,0 });
			static int scale = 100;
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

			auto cellSize = mDebugRender->getCellSize();
			if (ImGui::DragInt2("Cell Size", glm::value_ptr(cellSize)))
			{
				cellSize.x = cellSize.x < 0 ? 0 : cellSize.x;
				cellSize.y = cellSize.y < 0 ? 0 : cellSize.y;
			}
			mDebugRender->setCellSize(cellSize);

			if (ImGui::ColorEdit3("Grid Color", glm::value_ptr(mGridColor)))
			{
				mGridColor.w = 1;
				mDebugRender->setColor({ mGridColor });
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorUI::viewPort()
	{
		ImVec4 WHITE = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		static bool open = true;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, WHITE);
		ImGui::Begin("Viewport", &open, flags);

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
		ImGui::SetCursorPos({ x, y });

		ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

		float xPos = mapIn(ImGui::GetIO().MousePos.x - canvas_pos.x, 0, newSize.x, 0, winSize.x);
		float yPos = winSize.y - mapIn(ImGui::GetIO().MousePos.y - canvas_pos.y, 0, newSize.y, 0, winSize.y);

		ImGui::Image((void*)mFb.getTextureId(), { newSize.x, newSize.y }, { 0, 1 }, { 1, 0 }, WHITE, { 0.0, 0.0, 0.0, 1.0 });

		if (mCanvasHover = ImGui::IsItemHovered())
		{
			mouseGridCoords = mDebugRender->getSquareCoords({ xPos, yPos });

			if (mInput->onKeyPressed("R"))
				mCamera->setPosition(0, 0);

			if (mInput->onKeyPressed("Z"))
				mCamera->setScale(1);
			static Entity ent;
			if (mInput->onKeyPressed("MouseLeft"))
			{
				lastCoords = { xPos, yPos };
				lastCamPos = mCamera->getPosition();
				ent = mScene->getEntity(mPicker.getEntId({ xPos, yPos }));

				if (ent && mEnt != ent) {
					mEnt = ent;
					if (mEnt.hasComponent<Plutus::Transform>()) {
						auto trans = mEnt.getComponent<Plutus::Transform>();
						entLastPos = trans->getPosition();
					}
				}
			}
			// move the camera
			if (mInput->onKeyDown("Ctrl"))
			{
				if (mInput->onKeyDown("MouseLeft"))
				{
					glm::vec2 result = { xPos - lastCoords.x, yPos - lastCoords.y };
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
			else  if (mInput->onKeyDown("MouseLeft") && ent)
			{
				if (ent.hasComponent<Plutus::Transform>()) {
					auto trans = ent.getComponent<Plutus::Transform>();
					glm::vec2 result = { xPos - lastCoords.x, yPos - lastCoords.y };
					result /= mCamera->getScale();

					trans->x = entLastPos.x + result.x;
					trans->y = entLastPos.y + result.y;
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}

	void EditorUI::addRecent(const std::string& path)
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

	void EditorUI::onKeyDown(const std::string& key)
	{
	}

	void EditorUI::onKeyUp(const std::string& key)
	{
	}

	void EditorUI::drawMainDockingWin()
	{
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
		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &isOpen, window_flags);
		ImGui::PopStyleVar();


		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		/**************************************** Draw Editor Menu *******************************************************/
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					newScene();
				}
				if (ImGui::MenuItem("Open", "Ctrl+O", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					newScene();
					std::string path;
					if (Plutus::windowDialog(OPEN_FILE, path))
					{
						Plutus::SceneLoader::loadFromJson(path.c_str(), mScene);
						addRecent(path);
					}
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
							Plutus::SceneLoader::loadFromJson(recent.c_str(), mScene);
							addRecent(recent);
						}
					}
					ImGui::EndMenu();
					if (!toRemove.empty())
					{
						int index = Utils::getIndex(mRecents, toRemove);
						mRecents.erase(mRecents.begin() + index);
					}
				}
				if (ImGui::MenuItem("Save As", "Ctrl+S", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
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

		ImGui::End();
	}

	void EditorUI::bindFB()
	{
		drawScene();
		mRender.begin(&mShader, mCamera);

		mPicker.bind();
		mRender.draw(true);
		mPicker.unBind();

	}

	void EditorUI::unBindFB()
	{

		mFb.bind();
		mFb.setColor(mVPColor);
		mRender.draw();
		mRender.end();

		mDebugRender->drawGrid();
		mFb.unBind();
	}

	void EditorUI::saveScene()
	{
		std::string filePath;
		if (Plutus::windowDialog(SAVE_FILE, filePath))
		{
			std::string json = Plutus::sceneSerializer(mScene);
			Plutus::toJsonFile(filePath, json.c_str());
			addRecent(filePath);
		}
	}

	void addColor(Plutus::Serializer* sr, const char* label, const glm::vec4& color)
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
		auto cellSize = mDebugRender->getCellSize();
		writer->StartObject();
		writer->String("gridwidth");
		writer->Int(cellSize.x);
		writer->String("gridheight");
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
		Plutus::toJsonFile(path, sr.getString());
	}

	void EditorUI::drawScene()
	{
		auto viewMap = mScene->getRegistry()->view<TileMap>();
		auto view = mScene->getRegistry()->view<Transform, Sprite>();

		/******************Resize temp buffer************************/
		auto size = view.size_hint();

		for (auto [ent, map] : viewMap.each()) {
			size += map.mTiles.size();
		}

		if (mRenderables.size() != size + totalTemp) {
			mRenderables.resize(size + totalTemp);
		}

		/******************************************/

		int i = totalTemp;
		for (auto ent : viewMap)
		{
			auto [tilemap] = viewMap.get(ent);
			if (tilemap.mTiles.size())
			{
				auto tileset = tilemap.mTextures[0];
				const int w = tilemap.mTileWidth;
				const int h = tilemap.mTileHeight;

				for (auto& tile : tilemap.mTiles)
				{
					glm::vec4 rect{ tile.x, tile.y, w, h };
					if (mCamera->isBoxInView(rect, 200))
					{
						auto tileset = tilemap.mTextures[tile.texture];

						mRenderables[i++] = { tileset->texId, rect, tileset->getUV(tile.texcoord),
							{ tile.color }, tile.rotate, tile.flipX, tile.flipY, (int)entt::to_integral(ent), tilemap.mLayer, false };
					}
				}
			}
		}

		for (auto ent : view)
		{
			auto [trans, sprite] = view.get(ent);
			auto rect = trans.getRect();
			if (mCamera->isBoxInView(rect, 200))
			{
				mRenderables[i++] = { sprite.getTexId(), rect, sprite.getUV(), sprite.mColor,
					trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY };
			}

		}

		mRenderables.resize(i);
		// sort by layer, y position, texture
		std::sort(mRenderables.begin(), mRenderables.end());

		mRender.submit(mRenderables);
	}

	void initColor(const rapidjson::Document& doc, const char* label, glm::vec4& color)
	{
		if (doc.HasMember(label))
		{
			auto arr = doc[label].GetArray();
			color = { arr[0].GetFloat(), arr[0].GetFloat(), arr[0].GetFloat(), arr[0].GetFloat() };
		}
	}

	void EditorUI::loadRecents()
	{
		rapidjson::Document doc;
		if (loadJson("./pe-config.json", &doc))
		{
			if (doc.HasMember("gridwidth"))
			{
				mDebugRender->setCellSize({ doc["gridwidth"].GetInt(), doc["gridheight"].GetInt() });
			}
			initColor(doc, "vp-color", mVPColor);
			initColor(doc, "cell-color", mGridColor);

			if (doc["recents"].IsArray())
			{
				auto arr = doc["recents"].GetArray();

				for (uint32_t i = 0; i < arr.Size(); i++)
				{
					mRecents.push_back(arr[i].GetString());
					std::cout << arr[i].GetString() << std::endl;
				}

				if (mRecents.size())
				{
					auto path = mRecents[0];
					if (!path.empty())
					{
						Plutus::SceneLoader::loadFromJson(path.c_str(), mScene);
					}
				}
			}
		}
	}

	void EditorUI::newScene()
	{
		mScene->clear();
		mEnt = { entt::null, nullptr };
	}

} // namespace Plutus
