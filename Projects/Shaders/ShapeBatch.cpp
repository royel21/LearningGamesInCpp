#include "ShapeBatch.h"

#include <Math/PMath.h>
#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>

namespace Plutus
{

    const std::string shapeVShader = R"END(
        layout(location = 0) in vec2  vPosition;
        layout(location = 1) in vec2  vSize;
        layout(location = 2) in vec4  vColor;
        layout(location = 3) in float vRadius;
        layout(location = 4) in int vType;

        uniform mat4 uCamera;

        out vec4 colors;
        out vec2 sizes;
        out float radiuses;
        flat out int types;

        void main(){
            sizes = vSize;
            colors = vColor;
            radiuses = vRadius;
            types = vType;

            gl_Position = vec4(vPosition, 0., 1.);
        }
    )END";

    const std::string shapeGShader = R"END(
        layout(points) in;
        layout(triangle_strip, max_vertices = 4) out;

        in vec4 colors[];
        in vec2 sizes[];
        in float radiuses[];
        flat in int types[];

        out vec2 uv;
        out vec4 color;
        out vec2 size;
        out float radius; 
        flat out int type;

        uniform mat4 uCamera;

        void createVert(float x, float y, vec2 uUv){
            uv = uUv;
            color = colors[0];
            size = sizes[0];
            radius = radiuses[0];
            type = types[0];

            gl_Position = uCamera * (gl_in[0].gl_Position + vec4(x, y, 0.0, 0.0));
            EmitVertex();
        }

        void main()
        {
            vec4 uvCoords = vec4(-1., -1., 1., 1.);

            vec2 pSize = sizes[0];

            createVert(0, 0, uvCoords.xw);
            createVert(0, pSize.y, uvCoords.xy);
            createVert(pSize.x, 0, uvCoords.zw);
            createVert(pSize.x, pSize.y, uvCoords.zy);

            EndPrimitive();
        }
    )END";

    const std::string shapeFShader = R"END(
        in vec2 uv;
        in vec2 size;
        in vec4 color;
        in float radius;
        flat in int type;

        out vec4 fragColor;

        uniform float border;
        uniform vec4 borderColor;

        void main(){
            float alpha=0;
            float alpha2=0;
            
            switch(type){
                case 1:{
                    vec2 halfSize=size*.5;
                    vec2 pos=(abs(uv)*halfSize);
                    float rect=length(max(pos-(halfSize-radius),0.))-radius;
                    
                    alpha=1.-clamp(rect+border,0.,1.);
                    
                    if(border>0){
                        alpha2=1.-clamp(rect,0.,1.);
                    }
                    break;
                }
                default:{
                    float uSoftness=4./size.x;
                    float b=border/size.x;
                    
                    float dist=1.-length(uv);
                    alpha=smoothstep(uSoftness,uSoftness*1.5,dist-b);
                    
                    if(border>0){
                        alpha2=smoothstep(uSoftness,uSoftness*1.5,dist);
                    }
                }
            }
            if(border>0){
                fragColor=vec4(color*alpha)+vec4(borderColor*alpha2);
            }else{
                fragColor=vec4(color.rgb,alpha);
            }   
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
        Graphic::setFAttribute(2, 4, mVertexSize, offsetof(ShapeVert, color), GL_UNSIGNED_BYTE, GL_TRUE);
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(3, 1, mVertexSize, offsetof(ShapeVert, radius));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setIAttribute(4, 1, mVertexSize, offsetof(ShapeVert, type));

        Graphic::unBind();
    }

    void ShapeBatch::draw() {
        mShader.enable();
        mShader.setUniformMat4("uCamera", mCamera->getCameraMatrix());
        mShader.setUniform1f("border", mBorderWidth);
        mShader.setUniform4f("borderColor", mBorderColor);

        Graphic::bind(mVAO);

        Graphic::uploadBufferData(mBufferId, mShapes.size() * mVertexSize, mShapes.data());

        Graphic::drawArrays(GL_POINTS, mShapes.size());

        Graphic::unBind();
        mShader.disable();
        mShapes.clear();
    }

    void ShapeBatch::destroy() {
        Graphic::destroy(&mVAO, &mBufferId);
        mShader.destroy();
    }
} // namespace Plutus
