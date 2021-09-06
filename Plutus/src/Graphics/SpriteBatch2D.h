#ifndef _SpriteBatch2D_H
#define _SpriteBatch2D_H

#include <vector>

#include "vertex.h"
#include <unordered_map>


namespace Plutus
{
	struct Tile;
	class Shader;
	class Camera2D;
	class IndexBuffer;

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
		GLuint mVertexCount = 0;
		// Array of Rnder batcher per Image
		std::vector<RenderBatch2D> mRenderBatches;
		//Array of 4 Vertix per Single Object
		std::vector<Vertex> vertices;
		//Index Buffer Array Object
		IndexBuffer* mIBO = nullptr;
		// Camera with the screen coordinate where we are drawing
		Camera2D* mCamera = nullptr;
		Shader* mShader = nullptr;
		glm::vec4 camSize;
		bool mIsText = false;

	public:
		SpriteBatch2D() = default;
		~SpriteBatch2D();
		void init();
		//Prepare the Vertex buffer to add objects
		void begin(Shader* shader, Camera2D* camera, bool isText = false);
		//Reserve the memory for the objects
		void reserve(uint32_t size) {
			vertices.reserve(vertices.size() + (size << 2));
		};
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

		void submit(glm::vec4 rect, float r) { submit(0, rect, { 0,0,1,1 }, { 255,255,255 }, r); }
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
