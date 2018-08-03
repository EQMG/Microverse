#include "World.hpp"

#include <Post/Deferred/RendererDeferred.hpp>
#include <Shadows/Shadows.hpp>

namespace test
{
	World::World() :
		IModule(),
		m_noiseTerrain(Noise(69124)),
		m_fog(Fog(Colour::BLACK, 0.0f, 2.0f, -10000.0f, -1000.0f)),
		m_lightDirection(Vector3())
	{
		m_noiseTerrain.SetNoiseType(NoiseType::TYPE_PERLINFRACTAL);
		m_noiseTerrain.SetFrequency(0.00625f);
		m_noiseTerrain.SetInterp(NoiseInterp::INTERP_QUINTIC);
		m_noiseTerrain.SetFractalType(NoiseFractal::FRACTAL_FBM);
		m_noiseTerrain.SetFractalOctaves(4);
		m_noiseTerrain.SetFractalLacunarity(1.8f);
		m_noiseTerrain.SetFractalGain(0.6f);
	}

	World::~World()
	{
	}

	void World::Update()
	{
		m_lightDirection = Vector3(0.0124f, 0.992f, 0.124f);

		auto deferred = Renderer::Get()->GetManager()->GetRenderer<RendererDeferred>();

		if (deferred != nullptr)
		{
			deferred->SetFog(m_fog);
		}

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->SetLightDirection(m_lightDirection);
			Shadows::Get()->SetShadowBoxOffset(10.0f);
			Shadows::Get()->SetShadowBoxDistance(40.0f);
			Shadows::Get()->SetShadowTransition(5.0f);
			Shadows::Get()->SetShadowDarkness(0.6f);
		}
	}

	float World::GetTerrainRadius(const float &radius, const float &theta, const float &phi) const
	{
		float height = m_noiseTerrain.GetValue(radius * Maths::WrapRadians(theta), radius * Maths::WrapRadians(phi));
		return radius + (38.0f * height);
	}
}