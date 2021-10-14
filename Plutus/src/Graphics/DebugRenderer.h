#ifndef _DEBUGRENDERER_H
#define _DEBUGRENDERER_H

#include <vector>
#include <glm/glm.hpp>

#include "vertex.h"
#include "Shader.h"
#include "GLheaders.h"

#include <Physics/Shapes.h>

namespace Plutus
{
	class Camera2D;
	struct Box;
	struct Line;
	struct Circle;

	struct DebugVertex
	{
		glm::vec2 position;
		ColorRGBA8 color;
	};

	class DebugRender
	{
	public:
		DebugRender();
		static DebugRender* geInstances();
		~DebugRender();
		void init(Camera2D* _camera);
		void drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color = {});
		void drawBox(const glm::vec4& destRect, float angle = 0, const ColorRGBA8& color = {});
		void drawCircle(const glm::vec2& center, float radius, const ColorRGBA8& color = {});
		void render(float lineWidth = 1.0f);
		void drawGrid();
		void end();
		void dispose();
		//Grid Cell Size w = Width, h = Height
		void setCellSize(const glm::ivec2& cellSize);

		//Draw Shapes
		void drawBox(const Box2d& c, const ColorRGBA8& color = {});
		void drawLine(const Line2d& c, const ColorRGBA8& color = {});
		void drawCircle(const Circle2d& c, const ColorRGBA8& color = {});

		glm::ivec2 getCellSize() { return mCellSize; };

		inline void resizeBuffer(unsigned int size) { mVertexs.reserve(size); }

		glm::vec2 getSquareCoords(glm::vec2 mousePos);
		glm::vec2 getSquareCoords(glm::vec2 mousePos, const glm::vec2& size);

		void setShouldDraw(bool shouldDraw) { isDraw = shouldDraw; }

		void setColor(ColorRGBA8 color) { mGridColor = color; }

	private:
		glm::ivec2 mCellSize;

		bool isDraw = true;

		Shader mShader;
		std::vector<DebugVertex> mVertexs;
		std::vector<GLuint> mIndices;
		GLuint mVbo, mVao, mIbo;
		uint32_t mNumElements = 0;
		Camera2D* mCamera = nullptr;
		ColorRGBA8 mGridColor;
	};
} // namespace Plutus

#endif // !_DEBUGRENDERER_H