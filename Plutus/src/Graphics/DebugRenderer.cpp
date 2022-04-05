#include "DebugRenderer.h"

#include "GLSL.h"
#include "Camera2D.h"
#include "GraphicsUtil.h"
#include <algorithm>

#include <Math/PMath.h>

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

		mShader.destroy();
	}

	void DebugRender::init(Camera2D* _camera)
	{
		mCamera = _camera;
		mShader.init(GLSL::debug_vertshader, GLSL::debug_fragshader);
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
	void DebugRender::drawBox(Box2d& b, const ColorRGBA8& color)
	{
		// drawBox(vec4f(b.pos.x, b.pos.y, b.size.x, b.size.y), b.rotation, color);
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 4);
		auto vertices = b.getVertices();

		int index = 0;
		for (uint32_t j = i; j < i + 4; j++) {
			mVertexs[j].position = vertices[index++];
			mVertexs[j].color = color;
		}

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

	void DebugRender::drawLine(Line2d& line, const ColorRGBA8& color)
	{
		// drawLine({ l.pos.x, l.pos.y }, { l.end.x, l.end.y }, l.rotation, color);

		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 2);

		auto vertices = line.getVertices();

		mVertexs[i].position = vertices[0];
		mVertexs[i].color = color;
		mVertexs[i + 1].position = vertices[1];
		mVertexs[i + 1].color = color;

		mIndices.push_back(i);
		mIndices.push_back(i + 1);
	}

	void DebugRender::drawCircle(Circle2d& c, const ColorRGBA8& color)
	{
		drawCircle({ c.pos.x, c.pos.y }, c.radius, color);
	}
	/*******************************************************************************************************/

	void DebugRender::drawLine(const vec2f& a, const vec2f& b, float angle, const ColorRGBA8& color)
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
	void DebugRender::drawBox(const vec4f& rect, float angle, const ColorRGBA8& color)
	{
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 4);

		vec2f bl(rect.x, rect.y);
		//top left
		vec2f tl(rect.x, rect.y + rect.w);
		//bottom right
		vec2f tr(rect.x + rect.z, rect.y + rect.w);
		//top right
		vec2f br(rect.x + rect.z, rect.y);

		if (angle)
		{
			vec2f halfDim(rect.z * 0.5f, rect.w * 0.5f);
			vec2f center = vec2f(rect.x, rect.y) + halfDim;
			rotate(bl, center, angle);
			rotate(tl, center, angle);
			rotate(tr, center, angle);
			rotate(br, center, angle);
		}

		mVertexs[i + 0] = { bl, color };
		mVertexs[i + 1] = { tl, color };
		mVertexs[i + 2] = { tr, color };
		mVertexs[i + 3] = { br, color };

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

	void DebugRender::drawCircle(const vec2f& center, float radius, const ColorRGBA8& color)
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
			// glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glLineWidth(lineWidth);
			glBindVertexArray(mVao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
			glDrawElements(GL_LINES, mNumElements, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			mShader.disable();
		}
	}

	void DebugRender::drawGrid()
	{
		if (isDraw) {
			float tw = (float)mCellSize.x;
			float th = (float)mCellSize.y;

			auto orgScreenHalf = mCamera->getViewPortSize() / 2.0f;
			auto newPos = mCamera->getPosition() + orgScreenHalf;

			vec2f scaleScreen = mCamera->getScaleScreen();
			auto half = (scaleScreen / 2.0f);

			vec2f screenStart = newPos - half;
			vec2f screenEnd = newPos + half;
			vec2f lineStart;
			vec2f lineEnd;

			int sizeX = static_cast<int>((scaleScreen.x) / tw) + 2;
			int sizeY = static_cast<int>((scaleScreen.y) / th) + 2;

			float x = floor(screenStart.x / tw);
			float y = floor(screenStart.y / th);

			vec2f cPos(x * tw, y * th);

			for (int x = 0; x <= sizeX; x++)
			{
				lineStart.x = cPos.x + (x * tw);
				lineStart.y = cPos.y;

				lineEnd.x = cPos.x + (x * tw);
				lineEnd.y = screenEnd.y;
				drawLine(lineStart, lineEnd, 0, mGridColor);
			}

			for (int y = 0; y <= sizeY; y++)
			{
				lineStart.x = cPos.x;
				lineStart.y = cPos.y + (y * th);

				lineEnd.x = screenEnd.x;
				lineEnd.y = cPos.y + (y * th);
				drawLine(lineStart, lineEnd, 0, mGridColor);
			}
			end();

			render(1.0f);
		}
	}

	vec2i DebugRender::getSquareCoords(vec2f mousePos)
	{
		vec2f cmpos = mCamera->convertScreenToWold(mousePos);

		int x = (int)floor(cmpos.x / mCellSize.x);
		int y = (int)floor(cmpos.y / mCellSize.y);

		return { x, y };
	}

	vec2i DebugRender::getSquareCoords(vec2f mousePos, const vec2f& size)
	{
		vec2f cmpos = mCamera->convertScreenToWold(mousePos);

		int x = (int)floor(cmpos.x / mCellSize.x) * mCellSize.x;
		int y = (int)floor(cmpos.y / mCellSize.y) * mCellSize.y;
		return { x, y };
	}

	void DebugRender::setCellSize(const vec2i& cellSize)
	{
		mCellSize = { std::max(0, cellSize.x), std::max(0, cellSize.y) };
	}
} // namespace Plutus

namespace Plutus
{
}