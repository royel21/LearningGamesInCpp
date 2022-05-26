
#include "SpriteBatch2D.h"

#include <algorithm>
#include <iostream>

#include "Shader.h"
#include "Graphic.h"
#include "Camera2D.h"
#include "IndexBuffer.h"
#include "Renderables.h"

#include <Math/PMath.h>
#include <Utils/Utils.h>
#include <Assets/Assets.h>
#include <ECS/Components/SpriteComponent.h>
#include <ECS/Components/TileMapComponent.h>
#include <ECS/Components/TransformComponent.h>

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
		Graphic::destroy(&mVAO, &mVBO);
		delete mIBO;
	}

	void SpriteBatch2D::init(uint32_t maxSprite)
	{
		mVAO = Graphic::createVertexArray();
		mVBO = Graphic::createBufferArray();

		auto vsize = sizeof(Vertex);
		//bind the Shader position to the buffer object
		Graphic::setFAttribute(SHADER_VERTEX_INDEX, 2, vsize);
		//bind the Shader UV "Texture coordinate" to the buffer object
		Graphic::setFAttribute(SHADER_UV_INDEX, 2, vsize, offsetof(Vertex, uv));
		//bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
		Graphic::setFAttribute(SHADER_COLOR_INDEX, 4, vsize, offsetof(Vertex, color), GL_UNSIGNED_BYTE, GL_TRUE);
		//bind the Shader UV "Texture coordinate" to the buffer object
		Graphic::setFAttribute(SHADER_ENTITYID_INDEX, 1, vsize, offsetof(Vertex, entId));

		mIBO = new IndexBuffer(maxSprite);

		Graphic::unBind();

		mRenderBatches.clear();
		bufferVertices.clear();
		mShader.init();
	}

	void SpriteBatch2D::createVertices(uint32_t texture, const Vec4f& rect, Vec4f uv, ColorRGBA8 c, float r, bool flipX, bool flipY, uint32_t entId)
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

	void SpriteBatch2D::submit(uint32_t texture, const Vec4f& rect, Vec4f uv, ColorRGBA8 c, float r, bool flipX, bool flipY, uint32_t entId) {
		resize(4);
		createVertices(texture, rect, uv, c, r, flipX, flipY, entId);
	}

	void SpriteBatch2D::submit(const std::vector<Renderable>& renderables)
	{
		resize(renderables.size());
		for (auto r : renderables)
		{
			uint32_t texId = 0;

			if (r.texture) {
				texId = r.texture->mTexId;
			}

			createVertices(texId, r.trans, r.uv, r.color, r.r, r.flipX, r.flipY, r.entId);
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

		Graphic::uploadBufferData(mVBO, bufferVertices.size() * sizeof(Vertex), bufferVertices.data());

		Graphic::bind(mVAO);
		mIBO->bind();
	}

	void SpriteBatch2D::draw(BatchType type)
	{
		mShader.setUniform1i("type", type);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{

			auto& batch = mRenderBatches[i];
			mShader.setUniform1i("hasTexture", batch.texture);

			Graphic::bindTexture(batch.texture);
			Graphic::drawElements(batch.numVertices, batch.offset);
		}
	}

	void SpriteBatch2D::unBind() {
		Graphic::unBind();
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


	inline void SpriteBatch2D::createBatch(uint32_t texture)
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