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
		vec2f position;
		ColorRGBA8 color;
	};

	class DebugRender
	{
	public:
		DebugRender();
		static DebugRender* get();
		~DebugRender();
		void init(Camera2D* _camera);

		void render(float lineWidth = 1.0f);
		void drawGrid();
		void end();
		void dispose();
		//Grid Cell Size w = Width, h = Height
		void setCellSize(const vec2i& cellSize);
		vec4f getGridLineColor() { return mGridColor; }

		//Draw Shapes

		void drawLine(const vec2f& a, const vec2f& b, float angle = 0, const ColorRGBA8& color = {});
		void drawBox(const vec4f& destRect, float angle = 0, const ColorRGBA8& color = {});
		void drawCircle(const vec2f& center, float radius, const ColorRGBA8& color = {});

		void drawBox(Box2d& c, const ColorRGBA8& color = {});
		void drawLine(Line2d& c, const ColorRGBA8& color = {});
		void drawCircle(Circle2d& c, const ColorRGBA8& color = {});

		vec2i getCellSize() { return mCellSize; };

		inline void resizeBuffer(unsigned int size) { mVertexs.reserve(size); }

		vec2i getSquareCoords(vec2f mousePos);
		vec2i getSquareCoords(vec2f mousePos, const vec2f& size);

		void setShouldDraw(bool shouldDraw) { isDraw = shouldDraw; }
		bool getShouldDraw() { return isDraw; }

		void setColor(ColorRGBA8 color) { mGridColor = color; }

	private:
		vec2i mCellSize;

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