#include "ShapeBatch.h"

#include <Math/PMath.h>
#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>

namespace Plutus
{
    void ShapeBatch::init(uint32_t MAX_SPRITE) {
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(0, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(1, 2, mVertexSize, offsetof(ShapeVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(2, 4, mVertexSize, offsetof(ShapeVert, color), GL_UNSIGNED_BYTE, GL_TRUE);
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(3, 1, mVertexSize, offsetof(ShapeVert, radius));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(3, 1, mVertexSize, offsetof(ShapeVert, type));

        mIbo.init(MAX_SPRITE);

        Graphic::unBind();
    }

    void ShapeBatch::addSprite(const Vec4f& rect, ColorRGBA8 c, float radius, int type) {

        auto vertices = rect.getvertices();

        mShapes.emplace_back(vertices[0], mUv.x, mUv.w, c); // Bottom Left
        mShapes.emplace_back(vertices[1], mUv.x, mUv.y, c); // Top Left
        mShapes.emplace_back(vertices[2], mUv.z, mUv.y, c); // Top Right
        mShapes.emplace_back(vertices[3], mUv.z, mUv.w, c); // Bottom Left

    }

    void ShapeBatch::draw(Shader* shader) {

    }

    void ShapeBatch::destroy() {
        Graphic::destroy(&mVAO, &mBufferId);
        mIbo.destroy();
    }
} // namespace Plutus
