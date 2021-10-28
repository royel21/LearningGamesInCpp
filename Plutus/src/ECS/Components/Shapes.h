#pragma once
#include <Math/Vectors.h>
#include <Box2d/box2d.h>

namespace Plutus
{
    //Shapes Types
    enum STypes {
        Line = 0x01,
        Circle = 0x02,
        Box = 0x03
    };

    struct BShape {
        b2FixtureDef fixtureDef;
        STypes type;
    };

    struct bEgde : public BShape {
        b2EdgeShape shape;
        vec2f end;

        bEgde() {
            type = Line;
        }
    };

    struct bCircle : public BShape {
        float radius;
        b2CircleShape circleShapeB;

        bCircle() { type = Circle; }

    };
    struct bBox : public BShape {
        b2PolygonShape polygonShape;
        vec2f size;

        bBox() { type = Box; }
    };
} // namespace Plutus
