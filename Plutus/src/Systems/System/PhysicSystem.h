#pragma once
#include "System.h"
#include <box2d/box2d.h>
#include <Utils/types.h>
#include <Math/Vectors.h>


namespace Plutus {

	class PhysicSystem : public ISystem {
	public:
		PhysicSystem(Scene* scene, vec2f gravity = { 0.0f, -9.8f });

		void init();
		void update(float dt);
		void destroy();
	};
}