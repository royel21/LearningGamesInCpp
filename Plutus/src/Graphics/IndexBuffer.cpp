#include "IndexBuffer.h"
#include <assert.h>
#include <Graphics/Graphic.h>

namespace Plutus
{
	IndexBuffer::IndexBuffer(GLuint count) : mIboID(-1), mCount(count)
	{
		init(count);
	}

	void IndexBuffer::init(uint32_t count) {
		mCount = count * 6;

		int offset = 0;
		GLuint* indices = new GLuint[mCount];

		for (uint32_t i = 0; i < mCount; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		if (mIboID == -1) {
			mIboID = Graphic::createElementBuffer();
		}
		else {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

	void IndexBuffer::destroy() {
		if (mIboID)
			glDeleteBuffers(1, &mIboID);
	}

} // namespace Plutus