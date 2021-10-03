#pragma once

#include <map>
#include <cstring>
#include <vector>

#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <Graphics/Shader.h>
#include <Graphics/FrameBuffer.h>
#include <Graphics/SpriteBatch2D.h>

#include <Core/type.h>
#include <ECS/Scene.h>
#include <Input/InputListener.h>

#include <Panels/AssetsTab.h>
#include <Panels/EntityEditor.h>

namespace Plutus
{
	class Input;
	class Camera2D;
	class DebugRender;

	class EditorUI : InputListener
	{

	private:
		static EditorUI* mInstance;

		Input* mInput = nullptr;
		Camera2D* mCamera = nullptr;
		ImGuiIO* mImGui_IO = nullptr;
		DebugRender* mDebugRender = nullptr;

		Entity mEnt;

		FrameBuffer mFb;
		FrameBuffer mPicker;

		Shader mShader;
		SpriteBatch2D mRender;

		glm::vec4 mVPColor;
		glm::vec4 mGridColor;

		Ref<Scene> mScene;

		glm::vec2 lastCoords;
		glm::vec2 lastCamPos;
		glm::vec2 entLastPos;
		glm::ivec2 mouseGridCoords;

		float mVPScale = .9f;
		bool mMoveCam = false;
		bool mShowDemo = false;
		bool mCanvasHover = false;
		int totalTemp = 0;

		//Panels
		EntityEditor mEntityEditor;
		AssetsTab mAssetsTab;

		std::vector<std::string> mRecents;

		std::vector<Renderable> mRenderables;

	public:
		std::vector<ImVec2> Selectedtiles;

	public:
		static EditorUI* getInstance();

		~EditorUI();

		void Init(Camera2D* cam);
		void update(float dt);

		void drawUI();
		void beginUI();
		void endUI();
		void destroy();
		//Bind Framebuffer
		void bindFB();
		void unBindFB();

		void setTotalTemp(int total) { totalTemp = total; }
		std::vector<Renderable>& getRenderables() { return mRenderables; }

		const glm::vec2& getLastCoords() { return lastCoords; }
		void setLastCoord(const glm::vec2& coords) { lastCoords = coords; }

		bool isHover() { return ImGui::IsAnyItemHovered(); }

		void viewPortBGColor(float r, float b, float g, float a);

		Entity getEntity() { return mEnt; }
		void setEntity(Entity ent) { mEnt = ent; }

		void addRecent(const std::string& path);

		void drawDemo() { ImGui::ShowDemoWindow(); }

		//Input Listener
		void onKeyDown(const std::string& key);
		void onKeyUp(const std::string& key);

		bool isCanvaHover() { return mCanvasHover; }
		SpriteBatch2D* getRenderer() { return &mRender; }
		glm::ivec2 getGridCoords() { return mouseGridCoords; }

	private:
		EditorUI();
		//ImGui Panels
		void drawMainDockingWin();

		void viewPort();
		void viewPortControl();

		void newScene();
		void saveScene();

		void loadRecents();
		void saveRecents();

		void drawScene();
	};

} // namespace Plutus