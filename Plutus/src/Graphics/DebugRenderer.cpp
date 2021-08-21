#include "DebugRenderer.h"
#include "Camera2D.h"

const float PI = 3.14159265359f;

namespace
{
	const char* VERT_SRC = R"(#version 330
		layout(location = 0) in vec2 vertexPosition;
		layout(location = 1) in vec4 vertexColor;
		
		out vec2 fragmentPosition;
		out vec4 fragmentColor;

		uniform mat4 camera;

		void main() {
			//Set the x,y position on the screen
			gl_Position.xy = (camera * vec4(vertexPosition, 0, 1.0)).xy;
			//the z position is zero since we are in 2D
			gl_Position.z = 0.0;
    
			//Indicate that the coordinates are normalized
			gl_Position.w = 1.0;
    
			fragmentColor = vertexColor;
			fragmentPosition = vertexPosition;
		})";

	const char* FRAG_SRC = R"(#version 330 core
			//The fragment shader operates on each pixel in a given polygon

			in vec2 fragmentPosition;
			in vec4 fragmentColor;

			//This is the 3 component float vector that gets outputted to the screen
			//for each pixel.
			out vec4 color;

			void main() {

				color = fragmentColor;
			})";
} // namespace

namespace Plutus
{
	DebugRender* DebugRender::geInstances()
	{
		static DebugRender* instance = new DebugRender();
		return instance;
	}

	DebugRender::DebugRender() : mVao(0), mVbo(0), mIbo(0), mGridColor(0, 0, 0, 255), mCellCount(40, 24), mCellSize(32, 32)
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
		mShader.CreateProgWithShader(VERT_SRC, FRAG_SRC);
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

	void DebugRender::drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color = ColorRGBA8())
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

	glm::vec2 rotatePoint(glm::vec2 pos, float angle)
	{
		float cosAng = cos(angle);
		float sinAng = sin(angle);

		return {
			pos.x * cosAng - pos.y * sinAng,
			pos.x * sinAng + pos.y * cosAng
		};
	}

	void DebugRender::drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle)
	{
		uint32_t i = (uint32_t)mVertexs.size();
		mVertexs.resize(mVertexs.size() + 4);
		if (angle) {
			glm::vec2 halfDim(destRect.z / 2.0f, destRect.w / 2.0f);

			glm::vec2 tl(-halfDim.x, halfDim.y);
			glm::vec2 bl(-halfDim.x, -halfDim.y);
			glm::vec2 br(halfDim.x, -halfDim.y);
			glm::vec2 tr(halfDim.x, halfDim.y);

			glm::vec2 positionOffset(destRect.x, destRect.y);

			mVertexs[i].position = rotatePoint(tl, angle) + halfDim + positionOffset;
			mVertexs[i + 1].position = rotatePoint(bl, angle) + halfDim + positionOffset;
			mVertexs[i + 2].position = rotatePoint(br, angle) + halfDim + positionOffset;
			mVertexs[i + 3].position = rotatePoint(tr, angle) + halfDim + positionOffset;
		}
		else {
			mVertexs[i].position = { destRect.x,  destRect.y };
			mVertexs[i + 1].position = { destRect.x,  destRect.y + destRect.w };
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

	void DebugRender::drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius)
	{
		static const uint32_t NUmVERTS = 100;

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

	void DebugRender::drawGrid()
	{
		if (isDraw)
		{
			glm::ivec2 endLine = mCellCount * mCellSize;

			glm::ivec2 lineStart;
			glm::ivec2 lineEnd;

			for (int x = 0; x <= mCellCount.x; x++)
			{
				int curPoint = x * mCellSize.x;
				lineStart.x = curPoint;
				lineStart.y = 0;

				lineEnd.x = curPoint;
				lineEnd.y = endLine.y;
				drawLine(lineStart, lineEnd, mGridColor);
			}
			for (int y = 0; y <= mCellCount.y; y++)
			{
				int curPoint = y * mCellSize.y;
				lineStart.y = curPoint;
				lineStart.x = 0;

				lineEnd.y = curPoint;
				lineEnd.x = endLine.x;
				drawLine(lineStart, lineEnd, mGridColor);
			}
			end();

			render(1.0f);
		}
	}

	glm::vec2 DebugRender::getSquareCoords(glm::vec2 mousePos)
	{
		glm::vec2 cmpos = mCamera->convertScreenToWold(mousePos);
		int x = (int)floor(cmpos.x / mCellSize.x) * mCellSize.x;
		int y = (int)floor(cmpos.y / mCellSize.y) * mCellSize.y;
		return glm::vec2(x, y);
	}

	glm::vec2 DebugRender::getSquareCoords(glm::vec2 mousePos, const glm::vec2& size)
	{
		glm::vec2 cmpos = mCamera->convertScreenToWold(mousePos);

		int x = (int)floor(cmpos.x / mCellSize.x) * mCellSize.x;
		int y = (int)floor(cmpos.y / mCellSize.y) * mCellSize.y;
		return glm::vec2(x, y);
	}

	void DebugRender::setCellSize(int w, int h)
	{
		mCellSize.x = w;
		mCellSize.y = h;
	}
	void DebugRender::setCellCount(int w, int h)
	{
		mCellCount.x = w;
		mCellCount.y = h;
	};
} // namespace Plutus

namespace Plutus
{
}