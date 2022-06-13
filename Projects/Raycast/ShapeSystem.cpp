#include "ShapeSystem.h"

#include <ECS/Scene.h>
#include <Core/Project.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>

#include "ShapeComponent.h"

namespace Plutus
{
    ShapeSystem::~ShapeSystem()
    {
        mShader.destroy();
    }

    void ShapeSystem::init()
    {
        mShader.init("Projects/TestBed/ver.glsl", "Projects/TestBed/frag.glsl");

        mVAOId = Graphic::createVertexArray();
        mBuffId = Graphic::createBufferArray();
        mIBuffId = Graphic::createElementBuffer();
        Graphic::setFAttribute(0, 2, 2 * sizeof(float));

        Graphic::unBind();
    }

    void ShapeSystem::prepare()
    {
        auto view = mProject->scene->getView<ShapeComponent>();

        for (auto& [ent, shape] : view.each()) {
            uint32_t count = shape.Points.size();

            if (count) {
                mPoints.reserve(mPoints.size() + count + 1);

                mPoints.push_back(shape.pos);

                for (const auto& p : shape.Points) {
                    mPoints.push_back(shape.pos + p);
                }
            }
            createIndex(count + 1);
        }

        Graphic::uploadBufferData(mBuffId, mPoints.size() * sizeof(float) * 2, mPoints.data());
        Graphic::uploadBufferData(mIBuffId, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_DYNAMIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

        mPoints.clear();
    }

    void ShapeSystem::createIndex(uint32_t count)
    {
        auto start = mIndices.size();

        mIndices.reserve(start + count);
        for (uint32_t i = 0; i < count; i++) {
            mIndices.push_back(start + i);
        }
    }

    void ShapeSystem::update(float dt)
    {
        prepare();

        mShader.enable();
        mShader.setUniform4f("uColor", Vec4f(1, 1, 1, 1));
        mShader.setUniformMat4("uCamera", mCamera->getCameraMatrix());

        Graphic::bind(mVAOId, mIBuffId);
        Graphic::drawElements(mIndices.size(), 0, GL_LINE_LOOP);
        Graphic::unBind();

        mShader.disable();
        mIndices.clear();
    }
}// End Plutus namespace