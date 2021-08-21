#ifndef _SpriteBatch2D_H
#define _SpriteBatch2D_H

#include <vector>

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
	class IndexBuffer;
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
		//Vertext Array Buffer Id
		GLuint mVAO = 0;
		//Vertext Object Buffer Id
		GLuint mVBO = 0;
		// Represent how many Vertice was created
		GLuint mIndexCount = 0;
		// Array of Rnder batcher per Image
		std::vector<RenderBatch2D> mRenderBatches;
		//Array of 4 Vertix per Single Object
		std::vector<Vertex> vertices;
		//Index Buffer Array Object
		IndexBuffer* mIBO = nullptr;
		// Camera with the screen coordinate where we are drawing
		Camera2D* mCamera = nullptr;

	public:
		SpriteBatch2D() = default;
		~SpriteBatch2D();
		void init(Camera2D* camera);
		//Prepare the Vertex buffer to add objects
		void begin();
		//Reserve the memory for the objects
		void reserve(uint32_t size) { vertices.reserve(size << 2); };
		/*
			Submit a single Object to draw in the screen
			@param texture Texture image from where to draw
			@param rect rectangle with the position and size
			@param uv coordinate inside the texture
			@param c optional color
			@param r optional rotation
			@param flipX optional flip the image from X coordinate
			@param flipY optional flip the image from Y coordinate
		*/
		void submit(GLuint texture, glm::vec4 rect, glm::vec4 uv = { 0, 0, 1, 1 }, ColorRGBA8 c = {}, float r = 0, bool flipX = false, bool flipY = false);
		//Flush the Vertex buffer to the screen
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
				mRenderBatches.push_back({ mIndexCount, 6, texture });
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
		void createVertice(const glm::vec4& rect, const glm::vec4& uv, ColorRGBA8 c, float r = 0, bool flipX = false, bool flipY = false);
		/*
			rotate a point by a angle in degre
			@param pos point
			@param angle angle in degre
		*/
		static glm::vec2 rotatePoint(glm::vec2 pos, float angle);
	};

} // namespace Plutus
#endif // !_BATHRENDERER2D_H
