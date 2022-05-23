#ifndef _DEBUGRENDERER_H
#define _DEBUGRENDERER_H

#include <vector>
#include "vertex.h"
#include "Shader.h"
#include "GLheaders.h"

#include <Physics/Shapes.h>

#include <Math/Vectors.h>

namespace Plutus
{
	class Camera2D;
	struct Box;
	struct Line;
	struct Circle;

	struct DebugVertex
	{
		Vec2f position;
		ColorRGBA8 color;
	};

	class DebugRender
	{
	public:
		DebugRender();
		static DebugRender* get();
		~DebugRender();
		void init(Camera2D* camera);

		void render(float lineWidth = 1.0f);
		void drawGrid();
		void end();
		void dispose();
		//Grid Cell Size w = Width, h = Height
		void setCellSize(const Vec2i& cellSize);
		Vec4f getGridLineColor() { return mGridColor; }

		//Draw Shapes

		void drawLine(const Vec2f& a, const Vec2f& b, float angle = 0, const ColorRGBA8& color = {});
		void drawBox(const Vec4f& destRect, float angle = 0, const ColorRGBA8& color = {});
		void drawCircle(const Vec2f& center, float radius, const ColorRGBA8& color = {});

		void drawBox(Box2d& box, const ColorRGBA8& color = {});

		inline void drawLine(Line2d& line, const ColorRGBA8& color = {}) {
			auto vertices = line.getVertices();
			drawLine(vertices[0], vertices[1], 0, color);
		}

		inline void drawCircle(Circle2d& c, const ColorRGBA8& color = {}) {
			drawCircle({ c.pos.x, c.pos.y }, c.radius, color);
		}

		Vec2i getCellSize() { return mCellSize; };

		inline void resizeBuffer(unsigned int size) { mVertexs.reserve(size); }

		Vec2i getSquareCoords(Vec2f mousePos);

		void setShouldDraw(bool shouldDraw) { isDraw = shouldDraw; }
		bool getShouldDraw() { return isDraw; }

		void setColor(ColorRGBA8 color) { mGridColor = color; }

	private:
		void addIndices(int index);

	private:
		Vec2i mCellSize;

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