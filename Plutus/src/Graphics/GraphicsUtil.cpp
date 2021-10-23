#include "GraphicsUtil.h"


#define DEC2RA(dec) dec *(PI / 180.0f)

namespace Plutus {


    /*
        rotate a point by a angle in degre
        @param pos point
        @param angle angle in degre
    */
    vec2f rotatePoint2D(const vec2f& pos, float angle) {

        float rad = DEC2RA(-angle);
        float cosAng = cos(rad);
        float sinAng = sin(rad);

        return { pos.x * cosAng - pos.y * sinAng,
                pos.x * sinAng + pos.y * cosAng };
    }

}// end namespace Plutus