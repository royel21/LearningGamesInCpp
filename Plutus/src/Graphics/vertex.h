#ifndef _VERTEX_H
#define _VERTEX_H

#include <Utils/Utils.h>
#include <Math/Vectors.h>
#include <Utils/RGBAColor.h>

namespace Plutus
{
	struct UV
	{
		float u;
		float v;

		UV() : u(0), v(0) {}
		UV(float _u, float _v) : u(_u), v(_v) {}
	};
	/*
		Represent a single Vertex information:
		Position Y,X
		Texture UV
		Color
	*/
	struct Vertex
	{
		vec2f position; /* Positon X,Y */
		UV uv;	/* Texture coord from where to sample */
		ColorRGBA8 color; /* RGBA Color for the sample */
		float entId;

		Vertex() : position(), uv(), entId(0) {}
		Vertex(float x, float y, float uvX, float uvY, ColorRGBA8 c, float _entId = 0)
			: position(x, y), uv(uvX, uvY), color(c), entId(_entId) {}

		Vertex(vec2f pos, float uvX, float uvY, ColorRGBA8 c, float _entId = 0)
			: position(pos), uv(uvX, uvY), color(c), entId(_entId) {}

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			color.setColor(r, g, b, a);
		}
	};
	//An object containing all data need for creating vertice
	struct Renderable {
		// Texture Id
		uint32_t TexId;
		// Rectangle with position x,y and width, height
		vec4f trans;
		// Texture coords "UV"
		vec4f uv = { 0,0,1,1 };
		// Color
		ColorRGBA8 color = {};
		// Rotation
		float r = 0;
		// Flip the texture coord on x
		bool flipX = false;
		// Flip the texture coord on y
		bool flipY = false;
		// Entity Id for mouse picking
		int entId = -1;
		// Layer Id
		int layer = 0;
		// Sort in Y order
		bool sortY = false;
		Renderable() = default;
		Renderable(uint32_t texture, vec4f rect, vec4f _uv, ColorRGBA8 _c) : TexId(texture), trans(rect), uv(_uv), color(_c) {}

		Renderable(uint32_t texture, vec4f rect, vec4f _uv, ColorRGBA8 _c, float _r, bool fx, bool fy, int id, int _layer, bool sY = false)
			: TexId(texture), trans(rect), uv(_uv), color(_c), r(_r), flipX(fx), flipY(fy), entId(id), layer(_layer), sortY(sY) {
		}

		bool operator < (const Renderable& rend) const {
			if (sortY && rend.sortY) {
				return std::tie(layer, rend.trans.y, TexId) < std::tie(rend.layer, trans.y, TexId);
			}
			else {
				return std::tie(layer, TexId) < std::tie(rend.layer, rend.TexId);
			}
		}
	};
} // namespace Plutus

#endif