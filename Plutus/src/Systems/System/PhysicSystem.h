#pragma once
#include "System.h"

#include <tuple>
#include <vector>
#include <memory>

#include <box2d/box2d.h>
#include <Math/Vectors.h>

namespace Plutus {
	struct CollisionData {
		uint32_t entId;
		bool isSensor;
	};

	class PhysicSystem : public ISystem, public b2ContactListener {
	public:
		~PhysicSystem() { destroy(); };

		void init(Project* project);
		void update(float dt);
		void destroy();

		void BeginContact(b2Contact* contact) override;
		// Called when two fixtures cease to touch
		void EndContact(b2Contact* contact) override;

	private:
		b2World* mWorld = nullptr;
		std::vector<std::pair<CollisionData, CollisionData>> mCollisionsStart;
		std::vector<std::pair<CollisionData, CollisionData>> mCollisionsEnd;

		void reportCollision();
	};
}