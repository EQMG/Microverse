#pragma once

#include <Engine/Engine.hpp>
#include <Maths/Noise/Noise.hpp>
#include <Maths/Visual/DriverLinear.hpp>
#include <Maths/Vector3.hpp>
#include <Maths/Colour.hpp>
#include <Lights/Fog.hpp>

using namespace acid;

namespace micro
{
	/// <summary>
	/// A module used for managing the world.
	/// </summary>
	class World :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static World *Get() { return Engine::Get()->GetModule<World>(); }

		World();

		void Update() override;

		Fog GetFog() const { return m_fog; }

		Vector3f GetLightDirection() const { return m_lightDirection; }
	private:
		Fog m_fog;
		Vector3f m_lightDirection;
	};
}