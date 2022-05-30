#pragma once
#include "System.h"

#include <tuple>
#include <vector>
#include <memory>
#include <functional>

#include <box2d/box2d.h>
#include <Math/Vectors.h>

namespace Plutus {

	class ICollisionListener;

	struct CollisionData {
		uint32_t entId;
		bool isSensor;
	};

	class PhysicSystem : public ISystem, public b2ContactListener, public b2RayCastCallback {
	public:
		std::function<float(b2Fixture*, Vec2f, Vec2f, float)> mRayCallBack = nullptr;

		~PhysicSystem() { destroy(); };

		void init();
		void update(float dt);
		void destroy();

		void BeginContact(b2Contact* contact) override;
		// Called when two fixtures cease to touch
		void EndContact(b2Contact* contact) override;

		void CastRay(const Vec2f& start, const Vec2f& end);

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

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