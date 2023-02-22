#pragma once

#include "Shapes.h"

#include <Math/Vectors.h>

namespace Plutus
{
    namespace Collider {
        /**
         * @brief Circle vs Circle Collision
         *
         * @param circle1 Circle Shape
         * @param circle2  Circle Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Circle2d* circle1, Circle2d* circle2, MTV* mtv = nullptr);
        /**
        * @brief Circle vs Line Collision
        *
        * @param circle1 Circle Shape
        * @param line  Line Shape
        * @param mtv Min Translation Vector for SAT separation Struct Pointer
        * @return true
        * @return false
        */
        bool isColliding(Circle2d* circle, Line2d* line, MTV* mtv = nullptr);
        /**
         * @brief Circle vs Line Collision
         *
         * @param circle1 Circle Shape
         * @param Box  Line Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Circle2d* circle, Box2d* box, MTV* mtv = nullptr);
        /**
         * @brief Line vs Line Collision
         *
         * @param line Line Shape
         * @param line Line Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Line2d* line1, Line2d* line2, MTV* mtv = nullptr);
        /**
         * @brief Line vs Circle Collision
         *
         * @param line Line Shape
         * @param circle Circle Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Line2d* line, Circle2d* circle, MTV* mtv = nullptr);
        /**
         * @brief Line vs Circle Collision
         *
         * @param line Line Shape
         * @param box Box Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Line2d* line, Box2d* box, MTV* mtv = nullptr);
        /**
        * @brief Box vs Box Collision
        *
        * @param box Box Shape
        * @param box Box Shape
        * @param mtv Min Translation Vector for SAT separation Struct Pointer
        * @return true
        * @return false
        */
        bool isColliding(Box2d* box1, Box2d* box2, MTV* mtv = nullptr);
        /**
         * @brief Box vs Circle Collision
         *
         * @param box Box Shape
         * @param circle Circle Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Box2d* box, Circle2d* circle, MTV* mtv = nullptr);
        /**
         * @brief Box vs Line Collision
         *
         * @param box Box Shape
         * @param line Line Shape
         * @param mtv Min Translation Vector for SAT separation Struct Pointer
         * @return true
         * @return false
         */
        bool isColliding(Box2d* box, Line2d* line, MTV* mtv = nullptr);
    }
} // namespace Plutus