#pragma once

#include <vector>

#include <Math/Vectors.h>


class b2Body;

namespace Plutus
{
    constexpr int P_Edge = 0;
    constexpr int P_Circle = 1;
    constexpr int P_Box = 2;

    struct Fixture {
        int type;
        // default = 0, 0
        vec2f offset;
        // default = 1, 1
        vec2f size = { 1,1 };
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
        Fixture(int _type, vec2f _offset, vec2f _size, float f = 0.3f, float d = 1, float res = 0, bool _isSensor = false)
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
        Fixture(int _type, vec2f _offset, float r = 1.0f, float f = 0.3f, float d = 1, float res = 0, bool _isSensor = false)
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

        inline void addEdge(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false) {
            mFixtures.push_back({ P_Edge, offset, size, friction, density, restitution, isSensor, });
        }
        inline void addCircle(vec2f offset, float radius, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false) {
            mFixtures.push_back({ P_Circle, offset, radius, friction, density, restitution, isSensor, });
        }
        inline void addBox(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false) {
            mFixtures.push_back({ P_Box, offset, size, friction, density, restitution, isSensor, });
        }

    };
} // namespace Plutus