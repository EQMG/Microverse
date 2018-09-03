#pragma once

#include <memory>
#include <Maths/Colour.hpp>
#include <Textures/Texture.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace micro
{
	class Planet :
		public ICelestial
	{
	private:
		int m_seed; // The seed used to generate this planet.
		float m_radius; // The planets radius (m).
		float m_density; // The planets density (kg/m^3).
		float m_mass; // The planets mass (kg).
		float m_escapeVelocity; // The planets escape velocity (m/s).

		float m_axialTilt; // How tilted over the planet is, rotates prograde 0<i<90, rotates retrograde 90<i<180 (Degrees).
		float m_axialTropics; // The positions of the tropics, +/- (Degrees).
		float m_axialPolar; // The positions of the polar caps, +/- (Degrees).

		float m_innerRings; // The ring rule min bounds (Earth radius) += 0.2.
		float m_outterRings; // The ring rule max bounds (Earth radius) += 0.2.

		std::vector<Colour> m_heightmap;
		uint32_t m_heightmapSize;
	public:
		std::shared_ptr<Texture> test;
		static const float MEDIAN_RADIUS;
		static const float MEDIAN_DENSITY;
		static const float MEDIAN_MASS;
		static const float SQUARE_RADIUS_RATIO;

		Planet(const int &seed = 0, const float &radius = MEDIAN_RADIUS, const float &density = MEDIAN_DENSITY, const float &axialTilt = 0.0f);

		~Planet();

		void Start() override;

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;

		Colour GetColour(const Vector3 &cartesian);

		float GetRadius(const Vector3 &cartesian);

		int GetSeed() const { return m_seed; }

		float GetRadius() const { return m_radius; }

		float GetMass() const { return m_mass; }

		float GetDensity() const { return m_density; }
	private:
		void GenerateHeightmap();
	};
}
