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
		float m_radius;
		float m_density;
		float m_mass;
		float m_escapeVelocity;
		float m_solarLuminosity;
		float m_surfaceTemperature;
		Colour m_surfaceColour;

		float m_planetInnerLimit;
		float m_planetOuterLimit;
		float m_planetFrostLine;
		float m_habitableMin;
		float m_habitableMax;
	public:
		static const float MEDIAN_RADIUS;
		static const float MU_TO_M;
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
