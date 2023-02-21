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
		uint32_t end();
		void dispose();
		//Grid Cell Size w = Width, h = Height
		void setCellSize(const Vec2i& cellSize);
		Vec4f getGridLineColor() { return mGridColor; }

		//Draw Shapes
		void submitLine(const Vec2f& a, const Vec2f& b, float angle = 0, const ColorRGBA8& color = {});
		void submitBox(const Vec4f& destRect, float angle = 0, const ColorRGBA8& color = {});
		void submitCircle(const Vec2f& center, float radius, const ColorRGBA8& color = {});

		void submitBox(Box2d* box, const ColorRGBA8& color = {});

		inline void submitLine(Line2d* line, const ColorRGBA8& color = {}) {
			auto vertices = line->getVertices();
			submitLine(vertices[0], vertices[1], 0, color);
		}

		inline void submitCircle(Circle2d* c, const ColorRGBA8& color = {}) {
			submitCircle({ c->pos.x, c->pos.y }, c->radius, color);
		}

		inline void drawRect(const Vec4f& rect, const ColorRGBA8& color = {}) {
			submitBox(rect, 0, color);
			end();
			render();
		}

		inline void drawLine(const Vec2f& a, const Vec2f& b, float angle = 0, const ColorRGBA8& color = {}) {
			submitLine(a, b, angle, color);
			end();
			render();
		}

		Vec2i getCellSize() { return mCellSize; };

		inline void resizeBuffer(unsigned int size) { mVertexs.reserve(size); }

		Vec2i getSquareCoords(Vec2f mousePos);

		void setShouldDraw(bool shouldDraw) { isDraw = shouldDraw; }
		bool getShouldDraw() { return isDraw; }

		void setColor(Vec4f color) { mGridColor.setColor(color); }

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