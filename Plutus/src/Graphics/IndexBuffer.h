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
		~IndexBuffer() { destroy(); }

		void init(uint32_t count);
		void calculateIndex(uint32_t count);
		void bind() const;

		inline uint32_t getCount() { return mCount; }

		void unbind() const;

		void destroy();

		uint32_t getCount() const { return mCount; }
	};
} // namespace Plutus
#endif