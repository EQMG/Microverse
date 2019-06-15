#include "World.hpp"

#include <Post/Deferred/SubrenderDeferred.hpp>
#include <Shadows/Shadows.hpp>
#include <Graphics/Graphics.hpp>

namespace micro
{
	World::World() :
		m_fog{Colour::Black, 0.0f, 2.0f, -10000.0f, -1000.0f}
	{
	}

	void World::Update()
	{
		m_lightDirection = {0.0124f, 0.992f, 0.124f};

		if (auto deferred{Graphics::Get()->GetSubrender<SubrenderDeferred>()}; deferred != nullptr)
		{
			deferred->SetFog(m_fog);
		}

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->SetLightDirection(m_lightDirection);
			//Shadows::Get()->SetShadowBoxOffset(10.0f);
			//Shadows::Get()->SetShadowBoxDistance(40.0f);
			//Shadows::Get()->SetShadowTransition(5.0f);
			//Shadows::Get()->SetShadowDarkness(0.6f);
		}
	}
}