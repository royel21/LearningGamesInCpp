#ifndef _VERTEX_H
#define _VERTEX_H

#include <Utils/Utils.h>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>

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
		Vec2f position; /* Positon X,Y */
		UV uv;	/* Texture coord from where to sample */
		ColorRGBA8 color; /* RGBA Color for the sample */
		float entId;

		Vertex() : position(), uv(), entId(0) {}
		Vertex(float x, float y, float uvX, float uvY, ColorRGBA8 c, float _entId = 0)
			: position(x, y), uv(uvX, uvY), color(c), entId(_entId) {}

		Vertex(Vec2f pos, float uvX, float uvY, ColorRGBA8 c, float _entId = 0)
			: position(pos), uv(uvX, uvY), color(c), entId(_entId) {}

		inline void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		inline void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		inline void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			color.setColor(r, g, b, a);
		}
	};
} // namespace Plutus

#endif