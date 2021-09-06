
#include "SpriteBatch2D.h"

#include <algorithm>
#include <iostream>

#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"
#include "Utils/Utils.h"
#include "IndexBuffer.h"
#include "Camera2D.h"
#include "Shader.h"

#include "Time/Timer.h"

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_VERTEX_SIZE sizeof(Vertex)
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE *RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2

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

	void SpriteBatch2D::init()
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		//bind the Shader position to the buffer object
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*)NULL);
		//bind the Shader UV "Texture coordinate" to the buffer object
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*)offsetof(Vertex, uv));
		//bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
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

		vertices.resize(RENDERER_MAX_SPRITES * 4);
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


			vertices[mVertexCount] = { rect.x + tl.x, rect.y + tl.y, uv.x, uv.w, c };
			vertices[mVertexCount + 1] = { rect.x + bl.x, rect.y + bl.y, uv.x, uv.y, c };
			vertices[mVertexCount + 2] = { rect.x + br.x, rect.y + br.y, uv.z, uv.y, c };
			vertices[mVertexCount + 3] = { rect.x + tr.x, rect.y + tr.y, uv.z, uv.w, c };
		}
		else
		{
			vertices[mVertexCount] = { rect.x, rect.y, uv.x, uv.w, c };
			vertices[mVertexCount + 1] = { rect.x, rect.y + rect.w, uv.x, uv.y, c };
			vertices[mVertexCount + 2] = { rect.x + rect.z, rect.y + rect.w, uv.z, uv.y, c };
			vertices[mVertexCount + 3] = { rect.x + rect.z, rect.y, uv.z, uv.w, c };
		}
		mVertexCount += 4;
	}

	void SpriteBatch2D::begin(Shader* shader, Camera2D* camera, bool isText)
	{
		mIsText = isText;
		mCamera = camera;
		mShader = shader;
		camSize = mCamera->getViewPortDim();
	}

	void SpriteBatch2D::submit(GLuint texture, glm::vec4 rect, glm::vec4 uv, ColorRGBA8 c, float r, bool flipX, bool flipY)
	{
		// Check if is inside the view port
		if (rect.x + rect.z > camSize.x && rect.y + rect.w > camSize.y && rect.x < camSize.z && rect.y < camSize.w) {
			createBatch(texture);
			createVertice(rect, uv, c, r, flipX, flipY);
		}
	}

	void SpriteBatch2D::end()
	{

		mShader->enable();
		mShader->setUniform1b("isText", mIsText);
		mShader->setUniform1i("mySampler", 0);
		mShader->setUniformMat4("camera", mCamera->getCameraMatrix());
		glClearDepth(1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

		mIBO->bind();
		glBindVertexArray(mVAO);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			mShader->setUniform1i("hasTexture", mRenderBatches[i].texture);
			glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
			glDrawElements(GL_TRIANGLES, mRenderBatches[i].numVertices, GL_UNSIGNED_INT, (void*)(mRenderBatches[i].offset * sizeof(GLuint)));
		}

		//Clean up
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIBO->unbind();
		// vertices.clear();
		mRenderBatches.clear();

		mShader->disable();

		mVertexCount = 0;
		mIndexCount = 0;
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