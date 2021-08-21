#ifndef _DEBUGRENDERER_H
#define _DEBUGRENDERER_H

#include "vertex.h"
#include "Shader.h"
#include "GLheaders.h"
#include "glm/glm.hpp"
#include <vector>

namespace Plutus
{
	class Camera2D;

	struct DebugVertex
	{
		glm::vec2 position;
		ColorRGBA8 color;
	};

	class DebugRender
	{
	public:
		static DebugRender* geInstances();
		~DebugRender();
		void init(Camera2D* _camera);
		void drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color);
		void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
		void drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
		void render(float lineWidth);
		void drawGrid();
		void end();
		void dispose();
		//Grid Cell Size w = Width, h = Height
		void setCellSize(int w, int h);
		void setCellCount(int w, int h);

		glm::ivec2 getCellSize() { return mCellSize; };
		glm::ivec2 getCellCount() { return mCellCount; };

		inline void resizeBuffer(unsigned int size) { mVertexs.reserve(size); }

		glm::vec2 getSquareCoords(glm::vec2 mousePos);
		glm::vec2 getSquareCoords(glm::vec2 mousePos, const glm::vec2& size);

		void setShouldDraw(bool shouldDraw) { isDraw = shouldDraw; }

		void setColor(ColorRGBA8 color) { mGridColor = color; }

	private:
		DebugRender();

		glm::ivec2 mCellSize;
		glm::ivec2 mCellCount;

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