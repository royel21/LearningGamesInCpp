#include "DebugRenderer.h"

#include "GLSL.h"
#include "Camera2D.h"
#include "GraphicsUtil.h"

constexpr uint32_t NUmVERTS = 64;

namespace Plutus
{
	DebugRender* DebugRender::get()
	{
		static DebugRender* instance = new DebugRender();
		return instance;
	}

	DebugRender::DebugRender() : mVao(0), mVbo(0), mIbo(0), mGridColor(0, 0, 0, 255), mCellSize(32, 32)
	{
	}

	DebugRender::~DebugRender()
	{
		dispose();
	}

	void DebugRender::dispose()
	{
		if (mVao)
			glDeleteVertexArrays(1, &mVao);
		if (mVbo)
			glDeleteBuffers(1, &mVao);
		if (mIbo)
			glDeleteBuffers(1, &mVao);
		mShader.dispose();
	}

	void DebugRender::init(Camera2D* _camera)
	{
		mCamera = _camera;
		mShader.CreateProgWithShader(GLSL::debug_vertshader, GLSL::debug_fragshader);
		mShader.setAtribute("vertexPosition");
		mShader.setAtribute("vertexColor");

		//Set up buffer
		glGenVertexArrays(1, &mVao);
		glGenBuffers(1, &mVbo);
		glGenBuffers(1, &mIbo);

		glBindVertexArray(mVao);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}
	void DebugRender::end()
	{
		//Set Up Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);

		glBufferData(GL_ARRAY_BUFFER, mVertexs.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexs.size() * sizeof(DebugVertex), mVertexs.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Set up Index Buffer Array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mIndices.size() * sizeof(GLuint), mIndices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mNumElements = (uint32_t)mIndices.size();
		mIndices.clear();
		mVertexs.clear();
	}
	/***************************** Shapes **********************************************/
	void DebugRender::drawBox(const Box2d& b, const ColorRGBA8& color)
	{
		drawBox(glm::vec4(b.pos.x, b.pos.y, b.size.x, b.size.y), 0, color);
	}

	void DebugRender::drawLine(const Line2d& l, const ColorRGBA8& color)
	{
		drawLine({ l.pos.x, l.pos.y }, { l.end.x, l.end.y }, color);
	}

	void DebugRender::drawCircle(const Circle2d& c, const ColorRGBA8& color)
	{
		drawCircle({ c.pos.x, c.pos.y }, c.radius, color);
	}
	/*******************************************************************************************************/

	void DebugRender::drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color)
	{
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 2);

		mVertexs[i].position = a;
		mVertexs[i].color = color;
		mVertexs[i + 1].position = b;
		mVertexs[i + 1].color = color;

		mIndices.push_back(i);
		mIndices.push_back(i + 1);
	}
	void DebugRender::drawBox(const glm::vec4& destRect, float angle, const ColorRGBA8& color)
	{
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 4);
		if (angle)
		{
			glm::vec2 halfDim(destRect.z / 2.0f, destRect.w / 2.0f);

			glm::vec2 tl(-halfDim.x, halfDim.y);
			glm::vec2 bl(-halfDim.x, -halfDim.y);
			glm::vec2 br(halfDim.x, -halfDim.y);
			glm::vec2 tr(halfDim.x, halfDim.y);

			glm::vec2 positionOffset(destRect.x, destRect.y);

			mVertexs[i].position = rotatePoint2D(tl, angle) + halfDim + positionOffset;
			mVertexs[i + 1].position = rotatePoint2D(bl, angle) + halfDim + positionOffset;
			mVertexs[i + 2].position = rotatePoint2D(br, angle) + halfDim + positionOffset;
			mVertexs[i + 3].position = rotatePoint2D(tr, angle) + halfDim + positionOffset;
		}
		else
		{
			mVertexs[i].position = { destRect.x, destRect.y };
			mVertexs[i + 1].position = { destRect.x, destRect.y + destRect.w };
			mVertexs[i + 2].position = { destRect.x + destRect.z, destRect.y + destRect.w };
			mVertexs[i + 3].position = { destRect.x + destRect.z, destRect.y };
		}
		for (uint32_t j = i; j < i + 4; j++)
			mVertexs[j].color = color;

		mIndices.reserve(mIndices.size() + 8);

		mIndices.push_back(i);
		mIndices.push_back(i + 1);

		mIndices.push_back(i + 1);
		mIndices.push_back(i + 2);

		mIndices.push_back(i + 2);
		mIndices.push_back(i + 3);

		mIndices.push_back(i + 3);
		mIndices.push_back(i);
	}

	void DebugRender::drawCircle(const glm::vec2& center, float radius, const ColorRGBA8& color)
	{
		uint32_t start = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + NUmVERTS);
		for (size_t i = 0; i < NUmVERTS; i++)
		{
			float angle = ((float)i / NUmVERTS) * 2.0f * PI;
			mVertexs[start + i].position.x = cos(angle) * radius + center.x;
			mVertexs[start + i].position.y = sin(angle) * radius + center.y;
			mVertexs[start + i].color = color;
		}
		//Set up indixed drawing
		mIndices.reserve(mIndices.size() + NUmVERTS * 2);
		for (uint32_t i = 0; i < NUmVERTS - 1; i++)
		{
			mIndices.push_back(start + i);
			mIndices.push_back(start + i + 1);
		}

		mIndices.push_back(start + NUmVERTS - 1);
		mIndices.push_back(start);
	}

	void DebugRender::render(float lineWidth)
	{
		if (isDraw) {
			mShader.enable();
			mShader.setUniformMat4("camera", mCamera->getCameraMatrix());

			glLineWidth(lineWidth);
			glBindVertexArray(mVao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
			glDrawElements(GL_LINES, mNumElements, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			mShader.disable();
		}
	}

	// void DebugRender::drawGrid()
	// {
	// 	if (isDraw)
	// 	{
	// 		glm::ivec2 endLine = mCellCount * mCellSize;

	// 		glm::vec2 lineStart;
	// 		glm::vec2 lineEnd;

	// 		for (int x = 0; x <= mCellCount.x; x++)
	// 		{
	// 			int curPoint = x * mCellSize.x;
	// 			lineStart.x = (float)curPoint;
	// 			lineStart.y = 0.0f;

	// 			lineEnd.x = (float)curPoint;
	// 			lineEnd.y = (float)endLine.y;
	// 			drawLine(lineStart, lineEnd, mGridColor);
	// 		}
	// 		for (int y = 0; y <= mCellCount.y; y++)
	// 		{
	// 			int curPoint = y * mCellSize.y;
	// 			lineStart.y = (float)curPoint;
	// 			lineStart.x = 0.0f;

	// 			lineEnd.y = (float)curPoint;
	// 			lineEnd.x = (float)endLine.x;
	// 			drawLine(lineStart, lineEnd, mGridColor);
	// 		}
	// 		end();

	// 		render(1.0f);
	// 	}
	// }

	void DebugRender::drawGrid()
	{
		if (isDraw) {
			float tw = (float)mCellSize.x;
			float th = (float)mCellSize.y;

			auto orgScreenHalf = mCamera->getViewPortSize() / 2.0f;
			auto newPos = mCamera->getPosition() + orgScreenHalf;

			glm::vec2 scaleScreen = mCamera->getScaleScreen();
			auto half = (scaleScreen / 2.0f);

			glm::vec2 screenStart = newPos - half;
			glm::vec2 screenEnd = newPos + half;
			glm::vec2 lineStart;
			glm::vec2 lineEnd;

			int sizeX = static_cast<int>((scaleScreen.x) / tw) + 2;
			int sizeY = static_cast<int>((scaleScreen.y) / th) + 2;

			float x = floor(screenStart.x / tw);
			float y = floor(screenStart.y / th);

			glm::vec2 cPos(x * tw, y * th);

			for (int x = 0; x <= sizeX; x++)
			{
				lineStart.x = cPos.x + (x * tw);
				lineStart.y = cPos.y;

				lineEnd.x = cPos.x + (x * tw);
				lineEnd.y = screenEnd.y;
				drawLine(lineStart, lineEnd, mGridColor);
			}

			for (int y = 0; y <= sizeY; y++)
			{
				lineStart.x = cPos.x;
				lineStart.y = cPos.y + (y * th);

				lineEnd.x = screenEnd.x;
				lineEnd.y = cPos.y + (y * th);
				drawLine(lineStart, lineEnd, mGridColor);
			}
			end();

			render(1.0f);
		}
	}

	glm::vec2 DebugRender::getSquareCoords(glm::vec2 mousePos)
	{
		glm::vec2 cmpos = mCamera->convertScreenToWold(mousePos);

		int x = (int)floor(cmpos.x / mCellSize.x);
		int y = (int)floor(cmpos.y / mCellSize.y);

		return glm::vec2(x, y);
	}

	glm::vec2 DebugRender::getSquareCoords(glm::vec2 mousePos, const glm::vec2& size)
	{
		glm::vec2 cmpos = mCamera->convertScreenToWold(mousePos);

		int x = (int)floor(cmpos.x / mCellSize.x) * mCellSize.x;
		int y = (int)floor(cmpos.y / mCellSize.y) * mCellSize.y;
		return glm::vec2(x, y);
	}

	void DebugRender::setCellSize(const glm::ivec2& cellSize)
	{
		mCellSize = cellSize;
	}
} // namespace Plutus

namespace Plutus
{
}