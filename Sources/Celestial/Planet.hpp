#pragma once

#include <memory>
#include "ICelestial.hpp"

using namespace acid;

namespace test
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
	public:
		static const float MEDIAN_RADIUS;
		static const float MEDIAN_DENSITY;
		static const float MEDIAN_MASS;
		static const float SQUARE_RADIUS_RATIO;

		Planet(const int &seed = 0, const float &radius = MEDIAN_RADIUS, const float &density = MEDIAN_DENSITY);

		~Planet();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Planet"; };

		int GetSeed() const { return m_seed; }

		float GetRadius() const { return m_radius; }

		float GetMass() const { return m_mass; }

		float GetDensity() const { return m_density; }
	};
}
