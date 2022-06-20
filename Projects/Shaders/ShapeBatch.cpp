#include "ShapeBatch.h"

#include <Math/PMath.h>
#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>

namespace Plutus
{

    const std::string shapeVShader = R"END(
        layout(location = 0) in vec2  vPosition;
        layout(location = 1) in vec2  vUV;
        layout(location = 2) in vec2  vSize;
        layout(location = 3) in vec4  vColor;
        layout(location = 4) in float vRadius;
        layout(location = 5) in int vType;

        uniform mat4 uCamera;

        out vec2 uv;
        out vec2 size;
        out vec4 color;
        out float radius;
        flat out int type;

        void main(){
            uv = vUV;
            size = vSize;
            color = vColor;
            radius = vRadius;
            type = vType;

            gl_Position = uCamera * vec4(vPosition, 0, 1.0);
        }
    )END";

    const std::string shapeFShader = R"END(
        in vec2 uv;
        in vec2 size;
        in vec4 color;
        in float radius;
        flat in int type;

        out vec4 fragColor;

        void main(){
            float alpha;
            
            switch(type){
                case 1:{
                    vec2 pos=(abs(uv)*size*.5);
                    
                    alpha=1.-clamp(length(max(pos-(size*.5-radius),0.))-radius,0.,1.);
                    break;
                }
                default:{
                    float uSoftness=4./size.x;
                    
                    float dist=1.-length(uv);
                    alpha=smoothstep(uSoftness,uSoftness*2.,dist);
                }
            }
            fragColor=vec4(color.rgb,alpha*color.a);
        })END";

    void ShapeBatch::init(Camera2D* camera) {
        mCamera = camera;
        mShader.init(shapeVShader, shapeFShader);

        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(0, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(1, 2, mVertexSize, offsetof(ShapeVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(2, 2, mVertexSize, offsetof(ShapeVert, size));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(3, 4, mVertexSize, offsetof(ShapeVert, color), GL_UNSIGNED_BYTE, GL_TRUE);
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(4, 1, mVertexSize, offsetof(ShapeVert, radius));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setIAttribute(5, 1, mVertexSize, offsetof(ShapeVert, type));

        Graphic::unBind();
    }

    void ShapeBatch::addShape(const Vec4f& rect, ColorRGBA8 c, float radius, int type) {
        auto vertices = rect.getvertices();
        auto size = rect.getSize();

        mShapes.emplace_back(vertices[0], mUv.x, mUv.w, size, c, radius, type); // Bottom Left
        mShapes.emplace_back(vertices[1], mUv.x, mUv.y, size, c, radius, type); // Top Left
        mShapes.emplace_back(vertices[2], mUv.z, mUv.y, size, c, radius, type); // Top Right
        mShapes.emplace_back(vertices[3], mUv.z, mUv.w, size, c, radius, type); // Bottom Left
        mIndexCount += 6;
    }

    void ShapeBatch::draw() {
        mShader.enable();
        mShader.setUniformMat4("uCamera", mCamera->getCameraMatrix());

        Graphic::bind(mVAO);

        Graphic::uploadBufferData(mBufferId, mShapes.size() * mVertexSize, mShapes.data());
        if (mIndexCount != mIbo.getCount()) {
            mIbo.init(mIndexCount);
        }

        mIbo.bind();

        Graphic::drawElements(mIndexCount);

        Graphic::unBind();
        mShader.disable();
        mIndexCount = 0;
        mShapes.clear();
    }

    void ShapeBatch::destroy() {
        Graphic::destroy(&mVAO, &mBufferId);
        mIbo.destroy();
        mShader.destroy();
    }
} // namespace Plutus
