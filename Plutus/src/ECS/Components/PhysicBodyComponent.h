#pragma once

#include <vector>

#include <Math/Vectors.h>
#include <box2d/b2_math.h>

constexpr float PIXEL_SIZE = 32.0f;
//Pixel Per Meter
constexpr float PPM = 1 / PIXEL_SIZE;
//Meter Per Pixel
constexpr float MPP = PIXEL_SIZE;
//Half Meter Per Pixel
constexpr float HMPP = PPM / 2.0f;

class b2Body;

namespace Plutus
{
    inline b2Vec2 toWorld(const Vec2f& value) {
        return { value.x * PPM, value.y * PPM };
    }

    inline Vec2f fromWorld(b2Vec2 value) {
        return { value.x * MPP, value.y * MPP };
    }

    inline b2Vec2 tobVec2(const Vec2f& vec) { return { vec.x, vec.y }; }
    inline Vec2f toVec2f(const b2Vec2& vec) { return { vec.x, vec.y }; }

    constexpr int P_Edge = 0;
    constexpr int P_Circle = 1;
    constexpr int P_Box = 2;

    struct Fixture {
        int type;
        // default = 0, 0
        Vec2f offset;
        // default = 1, 1
        Vec2f size = { 1,1 };
        // default = 1
        float radius = 1;
        // default = 0.3
        float friction = 0.3f;
        // default = 1
        float density = 1.0f;
        // default = 0
        float restitution = 0;
        // default = false
        bool isSensor = false;
        Fixture(int _type) : type(_type) {};
        /**
         * @brief Construct a new Box Or Edge Fixture
         *
         * @param _type Fixture Type {P_Edge, P_Circle, P_Box}
         * @param _offset
         * @param _size Width, Height
         * @param f friction
         * @param d density = mass
         * @param res restitution or bounciness
         * @param _isSensor
         */
        Fixture(int _type, Vec2f _offset, Vec2f _size, float f = 0.3f, float d = 1, float res = 0, bool _isSensor = false)
            : type(_type), offset(_offset), size(_size), friction(f), density(d), restitution(res), isSensor(_isSensor) {  }

        /**
         * @brief Construct a new Circle Fixture
         *
         * @param _type Fixture Type {P_Edge, P_Circle, P_Box}
         * @param _offset
         * @param r radius
         * @param f friction
         * @param d density = mass
         * @param res restitution or bounciness
         * @param _isSensor
         */
        Fixture(int _type, Vec2f _offset, float r = 1.0f, float f = 0.3f, float d = 1, float res = 0, bool _isSensor = false)
            : type(_type), offset(_offset), friction(f), radius(r), density(d), restitution(res), isSensor(_isSensor) {  }
    };

    enum BodyType {
        StaticBody,
        KinematicBody,
        DynamicBody
    };

    struct PhysicBodyComponent
    {
        b2Body* mBody;

        std::vector<Fixture> mFixtures;
        BodyType mBodyType = StaticBody;

        PhysicBodyComponent(BodyType type = StaticBody) : mBodyType(type) {};

        inline Fixture& addFixture(int type) {
            mFixtures.push_back({ type });
            return mFixtures.back();
        };

        inline void addEdge(Vec2f offset, Vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false) {
            mFixtures.push_back({ P_Edge, offset, size, friction, density, restitution, isSensor, });
        }
        inline void addCircle(Vec2f offset, float radius, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false) {
            mFixtures.push_back({ P_Circle, offset, radius, friction, density, restitution, isSensor, });
        }
        inline void addBox(Vec2f offset, Vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false) {
            mFixtures.push_back({ P_Box, offset, size, friction, density, restitution, isSensor, });
        }

    };
} // namespace Plutus