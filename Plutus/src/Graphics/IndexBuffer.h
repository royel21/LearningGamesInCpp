#ifndef _INDEXBUFFER_H
#define _INDEXBUFFER_H

#include "GLheaders.h"
#include <cstdint>

namespace Plutus
{
	class IndexBuffer
	{
	private:
		GLuint mIboID = -1;
		GLuint mCount;

	public:
		IndexBuffer() = default;
		IndexBuffer(GLuint count);
		~IndexBuffer() { cleanUp(); }

		void init(uint32_t count);
		void bind() const;

		void unbind() const;

		void cleanUp();

		GLuint getCount() const { return mCount; }
	};
} // namespace Plutus
#endif