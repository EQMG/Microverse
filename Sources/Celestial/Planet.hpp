#pragma once

#include <memory>
#include <Maths/Colour.hpp>
#include <Graphics/Images/Image2d.hpp>
#include "Celestial.hpp"

using namespace acid;

namespace micro
{
	class Planet :
		public Celestial
	{
	public:
		static const float MedianRadius;
		static const float MedianDensity;
		static const float MedianMass;
		static const float SquareRadiusRatio;

		explicit Planet(const std::optional<uint64_t> &seed = std::nullopt, const float &radius = MedianRadius, const float &density = MedianDensity, const float &axialTilt = 0.0f);

		void Start() override;

		void Update() override;

		Colour GetColour(const Vector3f &cartesian);

		float GetRadius(const Vector3f &cartesian);

		const std::optional<uint64_t> &GetSeed() const { return m_seed; }

		const float &GetRadius() const override { return m_radius; }

		const float &GetMass() const override { return m_mass; }

		const float &GetDensity() const { return m_density; }

		friend const Metadata &operator>>(const Metadata &metadata, Planet &planet);

		friend Metadata &operator<<(Metadata &metadata, const Planet &planet);

	private:
		std::optional<uint64_t> m_seed; // The seed used to generate this planet.
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
	};
}
