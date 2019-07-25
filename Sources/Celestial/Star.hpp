#pragma once

#include <Maths/Colour.hpp>
#include "Celestial.hpp"

using namespace acid;

namespace micro
{
	class Star :
		public Celestial
	{
	public:
		static const float MedianRadius;
		static const float MedianDensity;
		static const float MedianMass;
		static const float AuToM;
		static const float GConstant;

		Star(const float &radius = MedianRadius, const float &density = MedianDensity);

		void Start() override;

		void Update() override;
		
		static Colour CalculateColour(const float &surfaceTemperature);

		const float &GetRadius() const override { return m_radius; }

		const float &GetMass() const override { return m_mass; }

		const Colour &GetColour() const { return m_surfaceColour; }

		friend const Metadata &operator>>(const Metadata &metadata, Star &star);

		friend Metadata &operator<<(Metadata &metadata, const Star &star);

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
	};
}
