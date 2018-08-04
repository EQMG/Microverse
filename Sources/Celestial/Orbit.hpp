#pragma once

#include <Objects/IComponent.hpp>
#include "ICelestial.hpp"
#include "Star.hpp"

using namespace acid;

namespace test
{
	class Orbit :
		public IComponent
	{
	private:
		float m_eccentricity; // The orbits deviation, 0<e<1.

		float m_semiMajorAxis; // The semi-major axis (AU).
		float m_semiMinorAxis; // The semi-minor axis (AU).

		float m_periapsis; // The orbits size periapsis (AU).
		float m_apoapsis; // The orbits size apoapsis (AU).
		float m_period; // The orbital period (Days).
		float m_velocity; // The objects velocity (km/s).

		float m_pitch; // The orbits inclination, orbits prograde 0<i<90, orbits retrograde 90<i<180 (Degrees).
		float m_yaw; // The orbits longitude of the ascending node from 0-360 (Degrees).
		float m_roll; // The orbits argument of periapsis from 0-360 (Degrees).
	public:
		Orbit(const float &eccentricity = 0.0f, const float &semiMajorAxis = 1.0f, const float &parentMass = Star::MEDIAN_MASS, const float &pitch = 0.0f, const float &yaw = 0.0f, const float &roll = 0.0f);

		~Orbit();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Orbit"; };
	};
}
