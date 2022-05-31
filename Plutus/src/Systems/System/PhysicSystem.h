#pragma once
#include "System.h"

#include <tuple>
#include <vector>
#include <memory>
#include <functional>

#include <box2d/box2d.h>
#include <Math/Vectors.h>
#include <ECS/Components.h>

namespace Plutus {

	constexpr float QUERYOFFSET = 2;

	class ICollisionListener;

	struct CollisionData {
		uint32_t entId;
		bool isSensor;
	};

	class PhysicSystem : public ISystem, public b2ContactListener, public b2RayCastCallback, public b2QueryCallback {
	public:
		std::function<float(b2Fixture*, Vec2f, Vec2f, float)> mRayCallBack = nullptr;
		std::function<bool(b2Fixture*)> mQueryCallBack = nullptr;

		~PhysicSystem() { destroy(); };

		void init();
		void update(float dt);
		void destroy();

		void BeginContact(b2Contact* contact) override;
		// Called when two fixtures cease to touch
		void EndContact(b2Contact* contact) override;

		void CastRay(const Vec2f& start, const Vec2f& end) {
			mWorld->RayCast(this, toWorld(start), toWorld(end));
		}

		void queryWorld(const Vec4f& rect) {
			b2AABB aabb;
			aabb.lowerBound = toWorld(rect.getLower() + QUERYOFFSET);
			aabb.upperBound = toWorld(rect.getUpper() - QUERYOFFSET);
			mWorld->QueryAABB(this, aabb);
		}

		bool ReportFixture(b2Fixture* fixture) override { return mQueryCallBack ? mQueryCallBack(fixture) : false; }

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
			return mRayCallBack ? mRayCallBack(fixture, fromWorld(point), fromWorld(normal), fraction) : 0;
		}

		void AddListener(ICollisionListener* listener) {
			mCollisionListener.push_back(listener);
		}

	private:
		b2World* mWorld = nullptr;
		std::vector<std::pair<CollisionData, CollisionData>> mCollisionsStart;
		std::vector<std::pair<CollisionData, CollisionData>> mCollisionsEnd;
		std::vector<ICollisionListener* > mCollisionListener;
	};
}