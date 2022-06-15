#pragma once
#include "System.h"

#include <tuple>
#include <vector>
#include <memory>
#include <functional>

#include <box2d/box2d.h>
#include <Math/Vectors.h>
#include <ECS/Components.h>

#include <Graphics/DebugRenderer.h>

namespace Plutus {

	using RayCastCallBack = std::function<float(b2Fixture*, Vec2f, Vec2f, float)>;

	constexpr float QUERYOFFSET = 2;

	class ICollisionListener;

	struct CollisionData {
		uint32_t entId;
		bool isSensor;
	};

	class ScriptSystem;

	class PhysicSystem : public ISystem, public b2ContactListener, public b2RayCastCallback, public b2QueryCallback {
	public:
		RayCastCallBack mRayCallBack = nullptr;

		~PhysicSystem() { destroy(); };

		void init();
		void update(float dt);
		void destroy();

		void BeginContact(b2Contact* contact) override;
		// Called when two fixtures cease to touch
		void EndContact(b2Contact* contact) override;

		inline void CastRay(const Vec2f& start, const Vec2f& end) {
			mWorld->RayCast(this, toWorld(start), toWorld(end));
		}

		inline void CastRay(const Vec2f& start, const Vec2f& end, RayCastCallBack callBack) {
			mRayCallBack = callBack;
			CastRay(start, end);
		}

		auto queryWorld(const Vec4f& rect, uint32_t mask = 0xffff) {
			mQueryMask = mask;
			mEntities.clear();
			b2AABB aabb;
			aabb.lowerBound = toWorld(rect.getLower() + QUERYOFFSET);
			aabb.upperBound = toWorld(rect.getUpper() - QUERYOFFSET);
			mWorld->QueryAABB(this, aabb);

			DebugRender::get()->drawRect(rect);

			return mEntities;
		}

		bool ReportFixture(b2Fixture* fix) override;

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
			return mRayCallBack ? mRayCallBack(fixture, fromWorld(point), fromWorld(normal), fraction) : 0;
		}

		inline void AddListener(ICollisionListener* listener) {
			mCollisionListener.push_back(listener);
		}

		void removeListener(ICollisionListener* listener);

	private:
		b2World* mWorld = nullptr;
		RayCastCallBack mRayScriptCallBack = nullptr;
		std::vector<std::pair<CollisionData, CollisionData>> mCollisionsStart;
		std::vector<std::pair<CollisionData, CollisionData>> mCollisionsEnd;
		std::vector<ICollisionListener*> mCollisionListener;
		std::vector<uint32_t> mEntities;
		uint32_t mQueryMask;
		friend ScriptSystem;
	};
}