#include "Star.hpp"

#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Graphics/Graphics.hpp>
#include <Post/Filters/FilterLensflare.hpp>

namespace micro
{
	const float Star::MedianRadius{2000.0f}; // (Originally 8000m) +- 37.5%
	const float Star::MedianDensity{1410.0f};
	const float Star::MedianMass{MedianDensity * (4.0f / 3.0f) * Maths::Pi<float> * std::pow(MedianRadius, 3.0f)};
	const float Star::AuToM{1.496e+11f};
	const float Star::GConstant{7.08398363e-7f}; // Real-world: 6.67430e−11f

	Star::Star(const float &radius, const float &density) :
		m_radius{radius},
		m_density{density},
		m_mass{m_density * (4.0f / 3.0f) * Maths::Pi<float> * std::pow(m_radius, 3.0f)},
		m_escapeVelocity{std::sqrt(2.0f * GConstant * m_mass / m_radius)},
		m_solarLuminosity{std::pow(m_mass / MedianMass, 3.5f)},
		m_surfaceTemperature{std::pow(m_solarLuminosity / std::pow(m_radius / MedianRadius, 2.0f), 0.25f) * 5778.0f},
		m_surfaceColour{CalculateColour(m_surfaceTemperature)},
		m_planetInnerLimit{0.1f * (m_mass / MedianMass)},
		m_planetOuterLimit{40.0f * (m_mass / MedianMass)},
		m_planetFrostLine{4.85f * std::sqrt(m_solarLuminosity)},
		m_habitableMin{std::sqrt(m_solarLuminosity / 1.11f)},
		m_habitableMax{std::sqrt(m_solarLuminosity / 0.53f)}
	{
		Log::Out("Star: Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Escape Velocity(m/s)=%f, Temperature(K)=%f, Surface Colour=%s, "
			"Planet Inner Limit(au)=%f, Planet Outer Limit(au)=%f, Planet Frost Line(au)=%f, Habitable Min(au)=%f, Habitable Max(au)=%f\n",
			m_radius, m_density, m_mass, m_escapeVelocity, m_surfaceTemperature, m_surfaceColour.GetHex(),
			m_planetInnerLimit, m_planetOuterLimit, m_planetFrostLine, m_habitableMin, m_habitableMax);
	}

	void Star::Start()
	{
		if (auto mesh{GetParent()->GetComponent<Mesh>()}; mesh != nullptr)
		{
			mesh->SetModel(ModelSphere::Create(m_radius, 30, 30));
		}
	}

	void Star::Update()
	{
		/*if (auto filterLensflare = Graphics::Get()->GetSubrender<FilterLensflare>(); filterLensflare != nullptr)
		{
			filterLensflare->SetSunPosition(GetParent()->GetWorldTransform().GetPosition());
			filterLensflare->SetSunHeight(1000.0f);
		}*/
	}

	Colour Star::CalculateColour(const float &surfaceTemperature)
	{
		auto temperature{std::clamp(surfaceTemperature, 1000.0f, 40000.0f) / 100.0f};
		float red, green, blue;

		if (temperature <= 66.0f)
		{
			red = 255.0f;
		}
		else
		{
			red = temperature - 60.0f;
			red = 329.698727446f * std::pow(red, -0.1332047592f);
			red = std::clamp(red, 0.0f, 255.0f);
		}

		if (temperature <= 66.0f)
		{
			green = temperature;
			green = 99.4708025861f * std::log(green) - 161.1195681661f;
			green = std::clamp(green, 0.0f, 255.0f);
		}
		else
		{
			green = temperature - 60.0f;
			green = 288.1221695283f * std::pow(green, -0.0755148492f);
			green = std::clamp(green, 0.0f, 255.0f);
		}

		if (temperature >= 66.0f)
		{
			blue = 255.0f;
		}
		else
		{
			if (temperature <= 19.0f)
			{
				blue = 0.0f;
			}
			else
			{
				blue = temperature - 10.0f;
				blue = 138.5177312231f * std::log(blue) - 305.0447927307f;
				blue = std::clamp(blue, 0.0f, 255.0f);
			}
		}

		return {red / 255.0f, green / 255.0f, blue / 255.0f};
	}

	const Metadata &operator>>(const Metadata &metadata, Star &star)
	{
		return metadata;
	}

	Metadata &operator<<(Metadata &metadata, const Star &star)
	{
		return metadata;
	}
}
