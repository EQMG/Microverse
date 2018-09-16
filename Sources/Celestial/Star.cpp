#include "Star.hpp"

#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Renderer/Renderer.hpp>
#include <Post/Filters/FilterLensflare.hpp>

namespace micro
{
	const float Star::MEDIAN_RADIUS = 2000.0f; // (Originally 8000m) +- 37.5%
	const float Star::MEDIAN_DENSITY = 1410.0f;
	const float Star::MEDIAN_MASS = MEDIAN_DENSITY * (4.0f / 3.0f) * PI * std::pow(MEDIAN_RADIUS, 3.0f);
	const float Star::AU_TO_M = 1.496e+11f;
	const float Star::G_CONSTANT = 7.08398363e-7f;

	Star::Star(const float &radius, const float &density) :
		m_radius(radius),
		m_density(density),
		m_mass(m_density * (4.0f / 3.0f) * PI * std::pow(m_radius, 3.0f)),
		m_escapeVelocity(std::sqrt(2.0f * G_CONSTANT * m_mass / m_radius)),
		m_solarLuminosity(std::pow(m_mass / MEDIAN_MASS, 3.5f)),
		m_surfaceTemperature(std::pow(m_solarLuminosity / std::pow(m_radius / MEDIAN_RADIUS, 2.0f), 0.25f) * 5778.0f),
		m_surfaceColour(CalculateColour(m_surfaceTemperature)),
		m_planetInnerLimit(0.1f * (m_mass / MEDIAN_MASS)),
		m_planetOuterLimit(40.0f * (m_mass / MEDIAN_MASS)),
		m_planetFrostLine(4.85f * std::sqrt(m_solarLuminosity)),
		m_habitableMin(std::sqrt(m_solarLuminosity / 1.11f)),
		m_habitableMax(std::sqrt(m_solarLuminosity / 0.53f))
	{
		Log::Out("Star: Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Escape Velocity(m/s)=%f, Temperature(K)=%f, Surface Colour=%s, "
				  "Planet Inner Limit(au)=%f, Planet Outer Limit(au)=%f, Planet Frost Line(au)=%f, Habitable Min(au)=%f, Habitable Max(au)=%f\n",
				  m_radius, m_density, m_mass, m_escapeVelocity, m_surfaceTemperature, m_surfaceColour.GetHex().c_str(),
			m_planetInnerLimit, m_planetOuterLimit, m_planetFrostLine, m_habitableMin, m_habitableMax);
	}

	void Star::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh != nullptr)
		{
			mesh->SetModel(ModelSphere::Resource(30, 30, m_radius));
		}
	}

	void Star::Update()
	{
		auto filterLensflare = Renderer::Get()->GetRenderer<FilterLensflare>();

		if (filterLensflare != nullptr)
		{
			filterLensflare->SetSunPosition(GetGameObject()->GetTransform().GetPosition());
			filterLensflare->SetSunHeight(1000.0f);
		}
	}

	void Star::Decode(const Metadata &metadata)
	{
	}

	void Star::Encode(Metadata &metadata) const
	{
	}

	Colour Star::CalculateColour(const float &surfaceTemperature)
	{
		float temperature = std::clamp(surfaceTemperature, 1000.0f, 40000.0f) / 100.0f;
		float red;
		float green;
		float blue;

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

		return Colour(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
}
