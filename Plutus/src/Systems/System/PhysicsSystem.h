#pragma once
#include "System.h"
#include <box2d/box2d.h>
#include <Utils/types.h>
#include <Math/Vectors.h>


namespace Plutus {

	class PhysicsSystem : public ISystem {
	public:
		PhysicsSystem(Scene* scene, vec2f gravity);

		void init();
		void update(float dt);
		void destroy();
	};
}