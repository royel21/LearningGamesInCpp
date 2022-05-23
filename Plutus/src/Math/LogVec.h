#include <Log/Logger.h>
#include "Vectors.h"

namespace Plutus {
    namespace Math
    {
        void Log(const Vec2f& v) {
            Logger::info("X:%.03f Y:%0.3f", v.x, v.y);
        }

        void Log(const Vec4f& v) {
            Logger::info("X:%.03f Y:%0.3f Z:%0.3f W:%0.3f", v.x, v.y, v.z, v.w);
        }
    } // namespace Math
}