#pragma once

#include <Engine/Engine.hpp>
#include <Noise/Noise.hpp>
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
		public IModule
	{
	private:
		Fog m_fog;
		Vector3 m_lightDirection;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static World *Get() { return Engine::Get()->GetModule<World>(); }

		World();

		void Update() override;

		Fog GetFog() const { return m_fog; }

		Vector3 GetLightDirection() const { return m_lightDirection; }
	};
}