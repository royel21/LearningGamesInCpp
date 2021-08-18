#ifndef _SpriteBatch2D_H
#define _SpriteBatch2D_H

#include <vector>

#include "IndexBuffer.h"
#include "vertex.h"
#include <unordered_map>

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_VERTEX_SIZE sizeof(Vertex)
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE *RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2

namespace Plutus
{
	class Camera2D;
	struct Tile;

	class RenderBatch2D
	{
	public:
		RenderBatch2D() = default;
		RenderBatch2D(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture)
		{
		}
		GLuint offset = 0;
		GLuint numVertices = 0;
		GLuint texture = 0;
	};

	class SpriteBatch2D
	{
	private:
		bool isSprite = false;
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIndexCount = 0;
		std::vector<RenderBatch2D> mRenderBatches;
		std::vector<Vertex> vertices;
		Vertex *mBuffer = nullptr;
		IndexBuffer *mIBO = nullptr;
		Camera2D *mCamera = nullptr;

	public:
		SpriteBatch2D();
		~SpriteBatch2D();
		void init(Camera2D *camera);
		void begin();
		void reserve(uint32_t size) { vertices.reserve(size << 2); };
		void submit(GLuint texture, glm::vec4 rect, glm::vec4 uv = {0, 0, 1, 1}, ColorRGBA8 c = {}, float r = 0, bool flipX = false, bool flipY = false);
		void end();

	private:
		/*
			Create a render Batch for this texture
		*/
		inline void createBatch(GLuint texture)
		{
			if (mRenderBatches.size() > 0 && mRenderBatches.back().texture == texture)
			{
				mRenderBatches.back().numVertices += 6;
			}
			else
			{
				mRenderBatches.push_back({mIndexCount, 6, texture});
			}

			mIndexCount += 6;
		}

		/*
			Create a Vertex Object with all the data needed for show an object on the screen
			@param x,y represent the position
			@param w,h represent the width and height
			@param uv vec4 for texture coordinate
			@param c is a RGBA color struct
			@param r is the rotaion - optional = 0
			@param flipx flip the texture in x direction
			@param flipy flip the texture in y direction
		*/
		void createVertice(const glm::vec4 &rect, const glm::vec4 &uv, ColorRGBA8 c, float r = 0, bool flipX = false, bool flipY = false);
		/*
			rotate a point by a angle in degre
			@param pos point
			@param angle angle in degre
		*/
		static glm::vec2 rotatePoint(glm::vec2 pos, float angle);
	};

} // namespace Plutus
#endif // !_BATHRENDERER2D_H
