#pragma once
#include <vector>

#include <Box2d/box2d.h>
#include <Physics/Shapes.h>
#include <Math/Vectors.h>
#include "../Scene.h"

class b2Body;

#pragma once

namespace Plutus
{
    enum BodyType {
        Static = b2_staticBody,
        Dynamic = b2_dynamicBody,
        Kinematic = b2_kinematicBody
    };

    struct Fixture {
        ShapeType type;

        vec2f offset;
        vec2f size;
        float radius;
        float friction = 0;
        float density = 1.0f;
        float restitution = 0;
        bool isSensor = false;
        Fixture(ShapeType _type, vec2f _offset, vec2f _size, float r, float f, float d, float res, bool _isSensor)
            : type(_type), offset(_offset), size(_size), friction(f), radius(r), density(d), restitution(res), isSensor(_isSensor)
        {

        }
    };

    class Body {

    protected:
        Entity mEnt;
        b2Body* mBody;
        BodyType bodyType;
        std::vector<Shape*> shapes;
        std::vector<Fixture> fixtures;
    public:
        vec2f offset;
        bool fixedRotation = true;
        float bullet = false;

    public:
        Body(Entity ent) { mEnt = ent; };
        ~Body();
        void init();
        void addEdge(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);
        void addCircle(vec2f offset, float radius, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);
        void addBox(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);

    };

    class BodyDynamic : public Body
    {
    public:
        BodyDynamic(Entity ent) : Body(ent) {};
        ~BodyDynamic() = default;
        BodyDynamic(Entity ent, const vec2f& _offset, float _mass = 0, float isFixedRotation = false, bool isBullet = false) : Body(ent) {
            offset = _offset;
            fixedRotation = isFixedRotation;
            bullet = isBullet;
            bodyType = Dynamic;
        }
    };

    class BodyKinematic : public Body
    {
    public:
        BodyKinematic(Entity ent) : Body(ent) {};
        ~BodyKinematic() = default;
        BodyKinematic(Entity ent, const vec2f& _offset, float isFixedRotation = false, bool isBullet = false) : Body(ent) {
            offset = _offset;
            fixedRotation = isFixedRotation;
            bullet = isBullet;
            bodyType = Kinematic;
        }
    };

    class BodyStatic : public Body
    {
    public:
        BodyStatic(Entity ent) : Body(ent) {};
        BodyStatic(Entity ent, const vec2f& _offset) : Body(ent) {
            offset = _offset;
            bodyType = Static;
        }
    };
} // namespace Plutus