
#include "SpriteBatch2D.h"

#include <algorithm>
#include <iostream>

#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/TileMapComponent.h"
#include <Utils/Utils.h>

#include "Shader.h"
#include "Camera2D.h"
#include "IndexBuffer.h"
#include "Graphic.h"

#include "GraphicsUtil.h"
#include <Math/PMath.h>
#include <Assets/Assets.h>

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2
#define SHADER_ENTITYID_INDEX 3


namespace Plutus
{
	SpriteBatch2D::~SpriteBatch2D()
	{
		mRenderBatches.clear();
		mShader.destroy();
		glDeleteBuffers(1, &mVBO);
		glDeleteVertexArrays(1, &mVAO);
		delete mIBO;
	}

	void SpriteBatch2D::init(uint32_t maxSprite)
	{
		mVAO = Graphic::createVertexArray();
		mVBO = Graphic::createBufferArray();
		//bind the Shader position to the buffer object
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)NULL);
		//bind the Shader UV "Texture coordinate" to the buffer object
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		//bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//
		glEnableVertexAttribArray(SHADER_ENTITYID_INDEX);
		glVertexAttribPointer(SHADER_ENTITYID_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, entId));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIBO = new IndexBuffer(maxSprite);

		glBindVertexArray(0);

		mRenderBatches.clear();
		bufferVertices.clear();
		mShader.init();
	}

	void SpriteBatch2D::createVertices(GLuint texture, const Vec4f& rect, Vec4f uv, ColorRGBA8 c, float r, bool flipX, bool flipY, GLuint entId)
	{
		// Check if is it inside the view port
		float id = static_cast<float>(entId);

		createBatch(texture);

		if (flipX)
			std::swap(uv.x, uv.z);

		if (flipY)
			std::swap(uv.y, uv.w);

		mbottomLeft = { rect.x, rect.y };
		mtopLeft = { rect.x, rect.y + rect.w };
		mtopRight = { rect.x + rect.z, rect.y + rect.w };
		mBottomRight = { rect.x + rect.z, rect.y };

		if (r)
		{
			Vec2f center(rect.x + rect.z * 0.5f, rect.y + rect.w * 0.5f);
			rotate(mbottomLeft, center, r);
			rotate(mtopLeft, center, r);
			rotate(mtopRight, center, r);
			rotate(mBottomRight, center, r);
		}

		bufferVertices[mVertexCount++] = { mbottomLeft.x, mbottomLeft.y, uv.x, uv.w, c, id };
		bufferVertices[mVertexCount++] = { mtopLeft.x, mtopLeft.y, uv.x, uv.y, c, id };
		bufferVertices[mVertexCount++] = { mtopRight.x, mtopRight.y, uv.z, uv.y, c, id };
		bufferVertices[mVertexCount++] = { mBottomRight.x, mBottomRight.y, uv.z, uv.w, c, id };
	}

	void SpriteBatch2D::submit(GLuint texture, const Vec4f& rect, Vec4f uv, ColorRGBA8 c, float r, bool flipX, bool flipY, uint32_t entId) {
		resize(4);
		createVertices(texture, rect, uv, c, r, flipX, flipY, entId);
	}

	void SpriteBatch2D::submit(const std::vector<Renderable>& renderables)
	{
		resize(renderables.size());
		for (auto r : renderables)
		{
			createVertices(r.TexId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY, r.entId);
		}
	}


	void SpriteBatch2D::submit(const std::string& fontId, const std::string& text, float x, float y, float scale, ColorRGBA8 color) {
		auto font = AssetManager::get()->getAsset<Font>(fontId);
		if (font != nullptr) {
			resize(text.size());

			for (auto i : text) {
				auto& ch = font->ch[i];
				GLfloat xpos = x + ch.bl * scale;
				GLfloat ypos = y - (ch.bh - ch.bt) * scale; // shift the letter down for Top-Left origin camera

				// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += ch.ax * scale;
				createVertices(font->mTexId, { xpos, ypos, ch.bw * scale, ch.bh * scale }, ch.uv, color);
			}
		}
	}

	void SpriteBatch2D::begin()
	{
		mShader.enable();
		mShader.setUniform1i("mySampler", 0);
		mShader.setUniformMat4("camera", mCamera->getCameraMatrix());

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(Vertex), bufferVertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(mVAO);
		mIBO->bind();
	}

	void SpriteBatch2D::draw(BatchType type)
	{
		mShader.setUniform1i("type", type);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0);
			auto& batch = mRenderBatches[i];
			mShader.setUniform1i("hasTexture", batch.texture);
			glBindTexture(GL_TEXTURE_2D, batch.texture);
			glDrawElements(GL_TRIANGLES, batch.numVertices, GL_UNSIGNED_INT, (void*)(batch.offset * sizeof(GLuint)));
		}
	}

	void SpriteBatch2D::unBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		mIBO->unbind();
		mShader.disable();
	}

	void SpriteBatch2D::end()
	{
		//Clean up
		unBind();
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