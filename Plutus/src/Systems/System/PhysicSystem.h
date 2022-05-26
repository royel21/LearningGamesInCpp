#pragma once
#include "System.h"
#include <box2d/box2d.h>
#include <Math/Vectors.h>
#include <memory>


namespace Plutus {

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
	};
}