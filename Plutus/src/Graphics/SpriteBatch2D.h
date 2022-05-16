#ifndef _SpriteBatch2D_H
#define _SpriteBatch2D_H

#include <vector>
#include <unordered_map>

#include "vertex.h"
#include "GLheaders.h"

#include <Math/Vectors.h>

#define DEF_UV Vec4f(0,0,1,1)

namespace Plutus
{
	struct Tile;
	class Shader;
	class Camera2D;
	class IndexBuffer;

	enum BatchType {
		BATCH_NONE = 0,
		BATCH_TEXT,
		BATCH_PICKING
	};

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
		//Array of 4 Vertix per Single Object
		std::vector<Vertex> bufferVertices;
		//Index Buffer Array Object
		IndexBuffer* mIBO = nullptr;
		// Camera with the screen coordinate where we are drawing
		Camera2D* mCamera = nullptr;
		Shader* mShader = nullptr;
		// Array of Renderables batcher per Image
		std::vector<Renderable> mRenderables;
		// Array of Rnder batcher per Image
		std::vector<RenderBatch2D> mRenderBatches;

	public:
		SpriteBatch2D() = default;
		~SpriteBatch2D();
		void init();
		//Prepare the Vertex buffer to add objects
		void begin();

		void setShader(Shader* shader) { mShader = shader; }
		void setCamera(Camera2D* cam) { mCamera = cam; }

		void submit(const std::vector<Renderable>& renderables);

		void submit(const Vec4f& rect, ColorRGBA8 c = {}, float r = 0) { resize(4); createVertices(0, rect, DEF_UV, c, r); }
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
		void submit(GLuint texture, const Vec4f& rect, Vec4f uv = DEF_UV, ColorRGBA8 c = {}, float r = 0, bool flipX = false, bool flipY = false, uint32_t entId = 0);

		void submit(const std::string& fontId, const std::string& text, float x, float y, float scale = 1, ColorRGBA8 color = {});

		void draw(BatchType type = BATCH_NONE);

		void unBind();
		//Flush the Vertex buffer to the screen
		void end();

		//draw to the screen
		inline void finish(BatchType type = BATCH_NONE) {
			begin();
			draw(type);
			end();
		}

	private:
		Vec2f mtopLeft;
		Vec2f mbottomLeft;
		Vec2f mtopRight;
		Vec2f mBottomRight;

		void createVertices(GLuint texture, const Vec4f& rect, Vec4f uv = DEF_UV, ColorRGBA8 c = {}, float r = 0, bool flipX = false, bool flipY = false, uint32_t entId = 0);
		/*
			Create a render Batch for this texture
		*/
		inline void createBatch(GLuint texture);

		inline void resize(int count) {
			auto size = count * 4 + mVertexCount;
			if (size > bufferVertices.size()) {
				bufferVertices.resize(size);
			}
		}
	};

} // namespace Plutus
#endif // !_BATHRENDERER2D_H
