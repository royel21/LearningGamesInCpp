#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "RigidBody2d.h"

namespace Plutus
{
    struct Collider2D
    {
        glm::vec2 offset;
    };

    struct Box2d
    {
        glm::vec2 size;
        glm::vec2 half;
        RigidBody2d* rBody = nullptr;

        Box2d()
        {
            half = size * 0.5f;
        }

        Box2d(const glm::vec2& min, const glm::vec2& max)
        {
            size = max - min;
            half = size * 0.5f;
        }

        glm::vec2 getMin()
        {
            return rBody->position - half;
        }

        glm::vec2 getMax()
        {
            return rBody->position + half;
        }

        std::vector<glm::vec2> getVertices()
        {
            glm::vec2 min = getMin();
            glm::vec2 max = getMax();

            return { {min.x, min.y}, {min.x, min.y}, {min.x, min.y}, {min.x, min.y} };
        }
    };

    struct AABB
    {
        glm::vec2 half;
        glm::vec2 size;
        RigidBody2d* rBody = nullptr;

        AABB()
        {
            half = size * 0.5f;
        }

        AABB(const glm::vec2& min, const glm::vec2& max)
        {
            size = max - min;
            half = size * 0.5f;
        }

        glm::vec2 getMin() const
        {
            return rBody->position - half;
        }

        glm::vec2 getMax() const
        {
            return rBody->position + half;
        }
    };

    struct Circle
    {
        RigidBody2d* rBody = nullptr;
        float radius = 1.0f;

        glm::vec2 getCenter() const {
            return rBody->position;
        }
    };
}