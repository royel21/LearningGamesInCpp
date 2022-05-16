#ifndef _INDEXBUFFER_H
#define _INDEXBUFFER_H

#include <cstdint>

namespace Plutus
{
	class IndexBuffer
	{
	private:
		uint32_t mIboID = -1;
		uint32_t mCount;

	public:
		IndexBuffer() = default;
		IndexBuffer(uint32_t count);
		~IndexBuffer() { cleanUp(); }

		void init(uint32_t count);
		void bind() const;

		void unbind() const;

		void cleanUp();

		uint32_t getCount() const { return mCount; }
	};
} // namespace Plutus
#endif