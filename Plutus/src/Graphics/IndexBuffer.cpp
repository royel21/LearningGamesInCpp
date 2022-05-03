#include "IndexBuffer.h"
#include <assert.h>

namespace Plutus
{
	IndexBuffer::IndexBuffer(GLuint count) : mIboID(0), mCount(count)
	{
		init(count);
	}

	void IndexBuffer::init(uint32_t count) {
		mCount = count * 6;

		int offest = 0;
		GLuint* indices = new GLuint[mCount];

		for (size_t i = 0; i < mCount; i += 6)
		{
			indices[i + 0] = offest + 0;
			indices[i + 1] = offest + 1;
			indices[i + 2] = offest + 2;

			indices[i + 3] = offest + 2;
			indices[i + 4] = offest + 3;
			indices[i + 5] = offest + 0;

			offest += 4;
		}

		glGenBuffers(1, &mIboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);

		delete[] indices;
	}

	void IndexBuffer::bind() const
	{
		if (mIboID)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::cleanUp() {
		if (mIboID)
			glDeleteBuffers(1, &mIboID);
	}

} // namespace Plutus