#pragma once

#include <Maths/Colour.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace test
{
	class Star :
		public ICelestial
	{
	private:
		float m_radius; // The stars radius (m).
		float m_density; // The stars density (kg/m^3).
		float m_mass; // The stars mass (kg).
		float m_escapeVelocity; // The stars escape velocity (m/s).
		float m_solarLuminosity; // The stars solar luminosity (L☉).
		float m_surfaceTemperature; // The stars surface temp in (K).
		Colour m_surfaceColour; // The stars surface colour (RGB).

		float m_planetInnerLimit; // The inner limit for planet formation (AU).
		float m_planetOuterLimit; // The outer limit for planet formation (AU).
		float m_planetFrostLine; // The planetary frost line for planets (AU).
		float m_habitableMin; // The habitable min distance for carbon based life (AU).
		float m_habitableMax; // The habitable max distance for carbon based life (AU).
	public:
		static const float MEDIAN_RADIUS;
		static const float AU_TO_M;
		static const float G_CONSTANT;

		Star(const float &radius = 8000.0f, const float &density = 1410.0f);

		~Star();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Star"; };

		float GetRadius() const { return m_radius; }

		float GetMass() const { return m_mass; }

		Colour GetColour() const { return m_surfaceColour; }
	private:
		Colour CalculateColour(const float &surfaceTemperature);
	};
}
