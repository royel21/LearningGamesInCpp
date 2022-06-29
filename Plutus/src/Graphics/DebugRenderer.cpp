#include "DebugRenderer.h"

#include "GLSL.h"
#include "Camera2D.h"
#include "Graphic.h"
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
		Graphic::destroy(&mVao, &mVbo, &mIbo);

		mShader.destroy();
	}

	void DebugRender::init(Camera2D* camera)
	{
		mCamera = camera;
		mShader.init(GLSL::debug_vertshader, GLSL::debug_fragshader);
		// //Set up buffer
		mVao = Graphic::createVertexArray();
		mVbo = Graphic::createBufferArray();
		mIbo = Graphic::createElementBuffer();
		auto vsize = sizeof(DebugVertex);

		Graphic::setFAttribute(0, 2, vsize);
		Graphic::setFAttribute(1, 4, vsize, offsetof(DebugVertex, color), GL_UNSIGNED_BYTE, GL_TRUE);

		Graphic::unBind();
	}

	void DebugRender::end()
	{
		Graphic::uploadBufferData(mVbo, mVertexs.size() * sizeof(DebugVertex), mVertexs.data());
		Graphic::uploadBufferData(mIbo, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_DYNAMIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

		mNumElements = (uint32_t)mIndices.size();
		mIndices.clear();
		mVertexs.clear();
	}

	void DebugRender::addIndices(int i) {
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
	/***************************** Shapes **********************************************/
	void DebugRender::submitBox(Box2d* b, const ColorRGBA8& color)
	{
		// submitBox(Vec4f(b.pos.x, b.pos.y, b.size.x, b.size.y), b.rotation, color);
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 4);
		auto vertices = b->getVertices();

		int index = 0;
		for (uint32_t j = i; j < i + 4; j++) {
			mVertexs[j].position = vertices[index++];
			mVertexs[j].color = color;
		}

		addIndices(i);
	}
	/*******************************************************************************************************/

	void DebugRender::submitLine(const Vec2f& a, const Vec2f& b, float angle, const ColorRGBA8& color)
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

	void DebugRender::submitBox(const Vec4f& rect, float angle, const ColorRGBA8& color)
	{
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 4);

		Vec2f bl(rect.x, rect.y);
		//top left
		Vec2f tl(rect.x, rect.y + rect.w);
		//bottom right
		Vec2f tr(rect.x + rect.z, rect.y + rect.w);
		//top right
		Vec2f br(rect.x + rect.z, rect.y);

		if (angle)
		{
			Vec2f halfDim(rect.z * 0.5f, rect.w * 0.5f);
			Vec2f center = Vec2f(rect.x, rect.y) + halfDim;
			PMath::rotate(bl, center, angle);
			PMath::rotate(tl, center, angle);
			PMath::rotate(tr, center, angle);
			PMath::rotate(br, center, angle);
		}

		mVertexs[i + 0] = { bl, color };
		mVertexs[i + 1] = { tl, color };
		mVertexs[i + 2] = { tr, color };
		mVertexs[i + 3] = { br, color };

		addIndices(i);
	}

	void DebugRender::submitCircle(const Vec2f& center, float radius, const ColorRGBA8& color)
	{
		uint32_t start = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + NUmVERTS);

		for (size_t i = 0; i < NUmVERTS; i++)
		{
			float angle = ((float)i / NUmVERTS) * 2.0f * PMath::PI;
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

	void DebugRender::drawGrid()
	{
		if (isDraw) {
			float tw = (float)mCellSize.x;
			float th = (float)mCellSize.y;

			auto campos = mCamera->getPosition();

			Vec2f size = mCamera->getScaleScreen();
			Vec2f lineStart;
			Vec2f lineEnd;

			Vec2f cpos = { round(campos.x / tw) * tw, round(campos.y / th) * th };

			for (float currentLine = -3 * tw; currentLine <= size.x + tw; currentLine += tw)
			{
				lineStart.x = cpos.x + currentLine;
				lineStart.y = cpos.y - th;

				lineEnd.x = cpos.x + (currentLine);
				lineEnd.y = cpos.y + size.y + th;
				submitLine(lineStart, lineEnd, 0, mGridColor);
			}

			for (float currentLine = -3 * th; currentLine <= size.y + th; currentLine += th)
			{
				lineStart.x = cpos.x - tw;
				lineStart.y = cpos.y + (currentLine);

				lineEnd.x = cpos.x + size.x + tw;
				lineEnd.y = cpos.y + (currentLine);
				submitLine(lineStart, lineEnd, 0, mGridColor);
			}
			end();

			render(1.45f);
		}
	}

	void DebugRender::render(float lineWidth)
	{
		if (isDraw) {
			mShader.enable();
			mShader.setUniformMat4("camera", mCamera->getCameraMatrix());
			glLineWidth(lineWidth);
			glEnable(GL_LINE_SMOOTH);

			Graphic::bind(mVao, mIbo);

			Graphic::drawElements(mNumElements, 0, GL_LINES);

			Graphic::unBind();

			mShader.disable();
		}
	}

	Vec2i DebugRender::getSquareCoords(Vec2f mousePos)
	{
		Vec2f cmpos = mCamera->convertScreenToWold(mousePos);

		int x = (int)floor(cmpos.x / mCellSize.x);
		int y = (int)floor(cmpos.y / mCellSize.y);

		return { x, y };
	}

	void DebugRender::setCellSize(const Vec2i& cellSize)
	{
		mCellSize = { std::max(0, cellSize.x), std::max(0, cellSize.y) };
	}
} // namespace Plutus

namespace Plutus
{
}