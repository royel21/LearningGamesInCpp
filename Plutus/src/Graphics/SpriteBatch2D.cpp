
#include "SpriteBatch2D.h"

#include <algorithm>
#include <iostream>

#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"
#include <Utils/Utils.h>

#include "IndexBuffer.h"
#include "Camera2D.h"
#include "Shader.h"

#include "GraphicsUtil.h"

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_VERTEX_SIZE sizeof(Vertex)
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE *RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2
#define SHADER_ENTITYID_INDEX 3

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
		//
		glEnableVertexAttribArray(SHADER_ENTITYID_INDEX);
		glVertexAttribPointer(SHADER_ENTITYID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*)offsetof(Vertex, entId));

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

	void SpriteBatch2D::submit(GLuint texture, glm::vec4 rect, glm::vec4 _uv, ColorRGBA8 c, float r, bool flipX, bool flipY, GLuint entId)
	{
		// Check if is it inside the view port
		float id = static_cast<float>(entId);

		createBatch(texture);

		glm::vec4 uv(_uv);

		if (flipX)
			std::swap(uv.x, uv.z);

		if (flipY)
			std::swap(uv.y, uv.w);

		if (r)
		{
			glm::vec2 halfDim(rect.z / 2, rect.w / 2);
			//top left
			glm::vec2 tl(-halfDim.x, halfDim.y);
			//bottom left
			glm::vec2 bl(-halfDim.x, -halfDim.y);
			//bottom right
			glm::vec2 br(halfDim.x, -halfDim.y);
			//top right
			glm::vec2 tr(halfDim.x, halfDim.y);

			tl = rotatePoint2D(tl, r) + halfDim;
			bl = rotatePoint2D(bl, r) + halfDim;
			br = rotatePoint2D(br, r) + halfDim;
			tr = rotatePoint2D(tr, r) + halfDim;

			vertices[mVertexCount + 0] = { rect.x + bl.x, rect.y + bl.y, uv.x, uv.w, c, id };
			vertices[mVertexCount + 1] = { rect.x + tl.x, rect.y + tl.y, uv.x, uv.y, c, id };
			vertices[mVertexCount + 2] = { rect.x + tr.x, rect.y + tr.y, uv.z, uv.y, c, id };
			vertices[mVertexCount + 3] = { rect.x + br.x, rect.y + br.y, uv.z, uv.w, c, id };
		}
		else
		{
			vertices[mVertexCount + 0] = { rect.x/*     */, rect.y/*     */, uv.x, uv.w, c, id };
			vertices[mVertexCount + 1] = { rect.x/*     */, rect.y + rect.w, uv.x, uv.y, c, id };
			vertices[mVertexCount + 2] = { rect.x + rect.z, rect.y + rect.w, uv.z, uv.y, c, id };
			vertices[mVertexCount + 3] = { rect.x + rect.z, rect.y/*     */, uv.z, uv.w, c, id };
		}
		mVertexCount += 4;
	}

	void SpriteBatch2D::submit(const std::vector<Renderable>& renderables)
	{
		//Resize to hold all the vertice needed
		auto size = renderables.size();

		size = (size ? size * 4 : 4) + mVertexCount;

		if (size > vertices.size()) {
			vertices.resize(size);
		}

		for (auto r : renderables)
		{
			submit(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY, r.entId);
		}
	}

	void SpriteBatch2D::begin(Shader* shader, Camera2D* camera, bool isText)
	{
		mCamera = camera;
		mShader = shader;
		shader->enable();
		shader->setUniform1b("isText", isText);
		shader->setUniform1i("mySampler", 0);
		shader->setUniformMat4("camera", mCamera->getCameraMatrix());

		glClearDepth(1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

		mIBO->bind();
		glBindVertexArray(mVAO);
	}

	void SpriteBatch2D::draw(bool usePicking)
	{
		mShader->setUniform1b("picking", usePicking);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			auto& batch = mRenderBatches[i];
			mShader->setUniform1i("hasTexture", batch.texture);
			glBindTexture(GL_TEXTURE_2D, batch.texture);
			glDrawElements(GL_TRIANGLES, batch.numVertices, GL_UNSIGNED_INT, (void*)(batch.offset * sizeof(GLuint)));
		}
	}

	void SpriteBatch2D::end()
	{
		//Clean up
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIBO->unbind();
		mShader->disable();
		mRenderBatches.clear();

		mVertexCount = 0;
		mIndexCount = 0;
	}

	inline void SpriteBatch2D::createBatch(GLuint texture)
	{
		if (mRenderBatches.size() > 0 && mRenderBatches.back().texture == texture)
		{
			mRenderBatches.back().numVertices += 6;
		}
		else
		{
			mRenderBatches.push_back({ mIndexCount, 6, texture });
		}

		mIndexCount += 6;
	}

} // namespace Plutus