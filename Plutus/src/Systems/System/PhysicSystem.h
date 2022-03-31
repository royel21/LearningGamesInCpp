#pragma once
#include "System.h"
#include <box2d/box2d.h>
#include <Utils/types.h>
#include <Math/Vectors.h>
#include <memory>


namespace Plutus {

	class PhysicSystem : public ISystem {
	public:
		PhysicSystem(Scene* scene);

		void init();
		void update(float dt);
		void destroy();
	private:
		b2World* mWorld = nullptr;
	};
}