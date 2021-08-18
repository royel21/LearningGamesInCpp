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

	DebugRender::DebugRender() : m_vao(0), m_vbo(0), m_ibo(0), mGridColor(0, 0, 0, 255), mCellCount(40, 24), mCellSize(32, 32)
	{
	}

	DebugRender::~DebugRender()
	{
		dispose();
	}
	void DebugRender::init(Camera2D* _camera)
	{
		m_camera = _camera;
		m_shader.CreateProgWithShader(VERT_SRC, FRAG_SRC);
		m_shader.setAtribute("vertexPosition");
		m_shader.setAtribute("vertexColor");

		//Set up buffer
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}
	void DebugRender::end()
	{
		//Set Up Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, m_vertexs.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexs.size() * sizeof(DebugVertex), m_vertexs.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Set up Index Buffer Array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numElements = (uint32_t)m_indices.size();
		m_indices.clear();
		m_vertexs.clear();
	}

	void DebugRender::drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color = ColorRGBA8())
	{
		uint32_t i = (uint32_t)m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + 2);

		m_vertexs[i].position = a;
		m_vertexs[i].color = color;
		m_vertexs[i + 1].position = b;
		m_vertexs[i + 1].color = color;

		m_indices.push_back(i);
		m_indices.push_back(i + 1);
	}

	glm::vec2 rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);

		return newV;
	}

	void DebugRender::drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle)
	{
		uint32_t i = (uint32_t)m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + 4);

		glm::vec2 halfDim(destRect.z / 2.0f, destRect.w / 2.0f);

		glm::vec2 tl(-halfDim.x, halfDim.y);
		glm::vec2 bl(-halfDim.x, -halfDim.y);
		glm::vec2 br(halfDim.x, -halfDim.y);
		glm::vec2 tr(halfDim.x, halfDim.y);

		glm::vec2 positionOffset(destRect.x, destRect.y);

		m_vertexs[i].position = rotatePoint(tl, angle) + halfDim + positionOffset;
		m_vertexs[i + 1].position = rotatePoint(bl, angle) + halfDim + positionOffset;
		m_vertexs[i + 2].position = rotatePoint(br, angle) + halfDim + positionOffset;
		m_vertexs[i + 3].position = rotatePoint(tr, angle) + halfDim + positionOffset;

		for (uint32_t j = i; j < i + 4; j++)
			m_vertexs[j].color = color;

		m_indices.reserve(m_indices.size() + 8);

		m_indices.push_back(i);
		m_indices.push_back(i + 1);

		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);

		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);

		m_indices.push_back(i + 3);
		m_indices.push_back(i);
	}

	void DebugRender::drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius)
	{
		static const uint32_t NUM_VERTS = 100;

		uint32_t start = (uint32_t)m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + NUM_VERTS);
		for (size_t i = 0; i < NUM_VERTS; i++)
		{
			float angle = ((float)i / NUM_VERTS) * 2.0f * PI;
			m_vertexs[start + i].position.x = cos(angle) * radius + center.x;
			m_vertexs[start + i].position.y = sin(angle) * radius + center.y;
			m_vertexs[start + i].color = color;
		}
		//Set up indixed drawing
		m_indices.reserve(m_indices.size() + NUM_VERTS * 2);
		for (uint32_t i = 0; i < NUM_VERTS - 1; i++)
		{
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}

		m_indices.push_back(start + NUM_VERTS - 1);
		m_indices.push_back(start);
	}

	void DebugRender::render(float lineWidth)
	{
		m_shader.enable();
		m_shader.setUniformMat4("camera", m_camera->getCameraMatrix());

		glLineWidth(lineWidth);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_shader.disable();
	}
	void DebugRender::dispose()
	{
		if (m_vao)
			glDeleteVertexArrays(1, &m_vao);
		if (m_vbo)
			glDeleteBuffers(1, &m_vao);
		if (m_ibo)
			glDeleteBuffers(1, &m_vao);
		m_shader.dispose();
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
		glm::vec2 cmpos = m_camera->convertScreenToWold(mousePos);
		int x = (int)floor(cmpos.x / mCellSize.x) * mCellSize.x;
		int y = (int)floor(cmpos.y / mCellSize.y) * mCellSize.y;
		return glm::vec2(x, y);
	}

	glm::vec2 DebugRender::getSquareCoords(glm::vec2 mousePos, const glm::vec2& size)
	{
		glm::vec2 cmpos = m_camera->convertScreenToWold(mousePos);

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