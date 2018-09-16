#include "World.hpp"

#include <Post/Deferred/RendererDeferred.hpp>
#include <Shadows/Shadows.hpp>
#include <Renderer/Renderer.hpp>

namespace micro
{
	World::World() :
		m_fog(Fog(Colour::BLACK, 0.0f, 2.0f, -10000.0f, -1000.0f)),
		m_lightDirection(Vector3())
	{
	}

	void World::Update()
	{
		m_lightDirection = Vector3(0.0124f, 0.992f, 0.124f);

		auto deferred = Renderer::Get()->GetRenderer<RendererDeferred>();

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
}