
#include "SpriteBatch2D.h"

#include <algorithm>
#include <iostream>

#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"
#include "Utils/Utils.h"
#include "IndexBuffer.h"
#include "Camera2D.h"

#include "Time/Timer.h"

#define PI 3.141592f
#define DEC2RA(dec) dec *(PI / 180.0f)

namespace Plutus
{
	SpriteBatch2D::~SpriteBatch2D()
	{
		mRenderBatches.clear();
		glDeleteBuffers(1, &mVBO);
		glDeleteVertexArrays(1, &mVAO);
		delete mIBO;
	}

	void SpriteBatch2D::init(Camera2D* camera)
	{
		mCamera = camera;
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		//Shader position
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*)NULL);
		//Shader UV "Texture coordinate"
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*)offsetof(Vertex, uv));
		//Shader Color
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (void*)offsetof(Vertex, color));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offest = 0;
		for (size_t i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i + 0] = offest + 0;
			indices[i + 1] = offest + 1;
			indices[i + 2] = offest + 2;

			indices[i + 3] = offest + 2;
			indices[i + 4] = offest + 3;
			indices[i + 5] = offest + 0;

			offest += 4;
		}

		mIBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);
		delete[] indices;
	}

	void SpriteBatch2D::begin()
	{
	}

	void SpriteBatch2D::createVertice(const glm::vec4& rect, const glm::vec4& _uv, ColorRGBA8 c, float r, bool flipX, bool flipY)
	{
		glm::vec4 uv(_uv);

		if (flipX)
			std::swap(uv.x, uv.z);

		if (flipY)
			std::swap(uv.y, uv.w);

		if (r)
		{
			glm::vec2 halfDim(rect.z / 2, rect.w / 2);

			glm::vec2 tl(-halfDim.x, halfDim.y);
			glm::vec2 bl(-halfDim.x, -halfDim.y);
			glm::vec2 br(halfDim.x, -halfDim.y);
			glm::vec2 tr(halfDim.x, halfDim.y);

			tl = rotatePoint(tl, r) + halfDim;
			bl = rotatePoint(bl, r) + halfDim;
			br = rotatePoint(br, r) + halfDim;
			tr = rotatePoint(tr, r) + halfDim;

			vertices.push_back({ rect.x + tl.x, rect.y + tl.y, uv.x, uv.w, c });
			vertices.push_back({ rect.x + bl.x, rect.y + bl.y, uv.x, uv.y, c });
			vertices.push_back({ rect.x + br.x, rect.y + br.y, uv.z, uv.y, c });
			vertices.push_back({ rect.x + tr.x, rect.y + tr.y, uv.z, uv.w, c });
		}
		else
		{
			vertices.push_back({ rect.x, rect.y, uv.x, uv.w, c });
			vertices.push_back({ rect.x, rect.y + rect.w, uv.x, uv.y, c });
			vertices.push_back({ rect.x + rect.z, rect.y + rect.w, uv.z, uv.y, c });
			vertices.push_back({ rect.x + rect.z, rect.y, uv.z, uv.w, c });
		}
	}

	void SpriteBatch2D::submit(GLuint texture, glm::vec4 rect, glm::vec4 uv, ColorRGBA8 c, float r, bool flipX, bool flipY)
	{
		createBatch(texture);
		createVertice(rect, uv, c, r, flipX, flipY);
	}

	void SpriteBatch2D::end()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

		mIBO->bind();
		glBindVertexArray(mVAO);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
			glDrawElements(GL_TRIANGLES, mRenderBatches[i].numVertices, GL_UNSIGNED_INT, (void*)(mRenderBatches[i].offset * sizeof(GLuint)));
		}

		//Clean up
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIBO->unbind();
		mIndexCount = 0;
		vertices.clear();
		mRenderBatches.clear();
	}

	glm::vec2 SpriteBatch2D::rotatePoint(glm::vec2 pos, float angle)
	{
		float rad = DEC2RA(angle);
		float cosAng = cos(rad);
		float sinAng = sin(rad);
		return { pos.x * cosAng - pos.y * sinAng,
				pos.x * sinAng + pos.y * cosAng };
	}

} // namespace Plutus