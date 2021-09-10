#pragma once

#include <map>
#include <cstring>
#include <vector>

#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Panels/EntityEditor.h"
#include "FrameBuffer.h"
#include "Panels/ComponentPanel.h"
#include "Graphics/SpriteBatch2D.h"
#include "Core/type.h"

namespace Plutus
{
	class Camera2D;
	class Scene;
	class DebugRender;
	class Input;
	class Entity;

	class EditorUI
	{
		struct props
		{
			bool flipX = false;
			bool flipY = false;
			int coord = 0;
		};

	private:
		static EditorUI* mInstance;

		Camera2D* mCamera = nullptr;
		ImGuiIO* mImGui_IO = nullptr;
		DebugRender* mDebugRender = nullptr;
		Input* mInput = nullptr;
		SpriteBatch2D mRender;
		FrameBuffer mFb;
		FrameBuffer mPicker;
		ImVec2 mViewportSize;
		glm::vec4 mVPColor;
		glm::vec4 mGridColor;
		//Panels
		EntityEditor mEntityEditor;
		Ref<Scene> mScene;
		Entity* mEnt = nullptr;

		bool mMoveCam = false;

		glm::vec2 lastCoords;
		glm::vec2 lastCamPos;
		glm::vec2 entLastPos;
		Entity* selectedEnt = nullptr;

		float mVPScale = .9f;
		bool mShowDemo = false;
		glm::ivec2 mouseGridCoords;

		std::vector<std::string> mRecents;
		bool mCanvasHover = false;

		ComponentPanel mComPanel;

	public:
		std::vector<ImVec2> Selectedtiles;

	public:
		static EditorUI* getInstance();

		~EditorUI();

		void Init(Camera2D* cam);
		void update(float dt);

		void DrawUI();
		void beginUI();
		void endUI();
		void destroy();
		//Bind Framebuffer
		void bindFB();
		void unBindFB();
		void resizeFB(int w, int h);
		void resizeFB(glm::vec2 size);

		ImGuiIO* getIO() { return mImGui_IO; }
		const glm::vec2& getLastCoords() { return lastCoords; }

		inline void setLastCoord(const glm::vec2& coords) { lastCoords = coords; }
		inline bool isHover() { return ImGui::IsAnyItemHovered(); }
		inline void drawDemo() { ImGui::ShowDemoWindow(); }
		void viewPortBGColor(float r, float b, float g, float a);
		void setEntity(Entity* ent) { mEnt = ent; }
		Entity* getEntity() { return mEnt; }
		void addRecent(const std::string& path);

	private:
		EditorUI();
		//ImGui Panels
		void drawMainDockingWin();
		void drawMenu(int dockspace_flags);
		void viewPort();
		void viewPortControl();
		void saveScene();
		void newScene();

		void loadRecents();
		void saveRecents();
	};

} // namespace Plutus