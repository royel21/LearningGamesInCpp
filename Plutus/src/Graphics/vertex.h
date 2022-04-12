#ifndef _VERTEX_H
#define _VERTEX_H

#include "GLheaders.h"

#include <Utils/Utils.h>
#include <Math/Vectors.h>

#define float2u8(c) static_cast<uint8_t>(c * 255)
#define RGBA2Int(r, g, b, a) a << 24 | b << 16 | g << 8 | r

namespace Plutus
{
	struct UV
	{
		float u;
		float v;

		UV() : u(0), v(0) {}
		UV(float _u, float _v) : u(_u), v(_v) {}
	};

	struct ColorRGBA8
	{
		uint32_t rgba;
		//Empty Constructor
		ColorRGBA8() : rgba(0xffffffff) {}

		ColorRGBA8(uint32_t color) : rgba(color) {}

		ColorRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { rgba = RGBA2Int(r, g, b, a); }

		ColorRGBA8(const vec4f& v) { setColor(v); }

		inline uint32_t get() { return rgba; };
		inline void setColor(uint32_t c) { rgba = c; }
		inline void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { rgba = RGBA2Int(r, g, b, a); }

		inline void setColor(const vec4f& v) {
			rgba = rgba = RGBA2Int(float2u8(v.x), float2u8(v.y), float2u8(v.z), float2u8(v.w));
		}

		operator uint32_t() const { return rgba; }
		//a << 24 | b << 16 | g << 8 | r
		operator vec4f() const {
			return {
				(rgba & 0xff) / 255.0f,
				((rgba >> 8) & 0xff) / 255.0f,
				((rgba >> 16) & 0xff) / 255.0f,
				((rgba >> 24) & 0xff) / 255.0f
			};
		}

		ColorRGBA8& operator=(const vec4f& v) {
			setColor(v);
			return *this;
		}
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