#pragma once

#include <Engine/Engine.hpp>
#include <Noise/Noise.hpp>
#include <Maths/Visual/DriverLinear.hpp>
#include <Maths/Vector3.hpp>
#include <Maths/Colour.hpp>
#include <Lights/Fog.hpp>

using namespace acid;

namespace test
{
	/// <summary>
	/// A module used for managing the world.
	/// </summary>
	class World :
		public IModule
	{
	private:
		Noise m_noiseTerrain;

		Fog m_fog;

		Vector3 m_lightDirection;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static World *Get()
		{
			return Engine::Get()->GetModule<World>();
		}

		/// <summary>
		/// Creates a new worlds module.
		/// </summary>
		World();

		/// <summary>
		/// Deconstructor for the worlds module.
		/// </summary>
		~World();

		void Update() override;

		float GetTerrainRadius(const float &radius, const float &theta, const float &phi) const;

		Noise GetTerrainNoise() const { return m_noiseTerrain; }

		Fog GetFog() const { return m_fog; }

		Vector3 GetLightDirection() const { return m_lightDirection; }
	};
}