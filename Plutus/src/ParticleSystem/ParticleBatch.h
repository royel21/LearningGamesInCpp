#ifndef _PARTICLEBATCH_H
#define _PARTICLEBATCH_H

#include <functional>

#include <Graphics/vertex.h>
#include <Math/Vectors.h>

namespace Plutus
{
	struct GLTexture;
	class SpriteBatch2D;

	struct Particle2D
	{
		vec2f m_position;
		vec2f m_velocity;
		ColorRGBA8 m_color;
		float m_life;
		float m_width;
	};

	inline void defaultParticleUpdate(Particle2D& p, float deltaTime)
	{
		p.m_position += p.m_velocity * deltaTime;
	}

	class ParticleBatch
	{
	private:
		float m_decayRate;
		int m_maxParticles;
		int m_lastParticle;
		GLTexture* m_texture = nullptr;
		Particle2D* m_particles;
		std::function<void(Particle2D&, float)> m_updatefunc;

	public:
		ParticleBatch();
		~ParticleBatch();

		void init(
			int maxParticles,
			float dr,
			GLTexture* tx,
			std::function<void(Particle2D&, float)> updatefunc = defaultParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch2D* sp);

		void addParticles(
			const vec2f& position,
			const vec2f& velocity,
			const ColorRGBA8& color, float width);

	private:
		int getLastParticle();
	};

} // namespace Plutus

#endif
