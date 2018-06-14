#include "MainWorld.hpp"

#include <Scenes/Scenes.hpp>
#include <Shadows/Shadows.hpp>

namespace test
{
	static const Colour FOG_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour FOG_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour FOG_COLOUR_DAY = Colour("#e6e6e6");

	static const Colour SUN_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour SUN_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour SUN_COLOUR_DAY = Colour("#ffffff");

	static const Colour MOON_COLOUR_NIGHT = Colour("#666699");
	static const Colour MOON_COLOUR_DAY = Colour("#000000");

	static const Colour SKYBOX_COLOUR_DAY = Colour("#003C8A");

	MainWorld::MainWorld() :
		m_noiseTerrain(Noise(69124)),
		m_driverDay(DriverLinear(0.0f, 1.0f, 300.0f)),
		m_factorDay(0.0f),
		m_skyboxRotation(Vector3()),
		m_sunPosition(Vector3()),
		m_moonPosition(Vector3()),
		m_sunColour(Colour()),
		m_moonColour(Colour()),
		m_fog(Fog(Colour::WHITE, 0.001f, 2.0f, -0.1f, 0.3f)),
		m_skyColour(Colour("#3399ff"))
	{
		m_noiseTerrain.SetNoiseType(NoiseType::TYPE_PERLINFRACTAL);
		m_noiseTerrain.SetFrequency(0.003f);
		m_noiseTerrain.SetInterp(NoiseInterp::INTERP_QUINTIC);
		m_noiseTerrain.SetFractalType(NoiseFractal::FRACTAL_FBM);
		m_noiseTerrain.SetFractalOctaves(5);
		m_noiseTerrain.SetFractalLacunarity(2.0f);
		m_noiseTerrain.SetFractalGain(0.5f);

		m_driverDay.Update(50.0f); // Starts during daytime.
	}

	MainWorld::~MainWorld()
	{
	}

	void MainWorld::Update()
	{
		float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay.Update(delta);

		m_skyboxRotation = Vector3(360.0f * m_factorDay, 0.0f, 0.0f);

		Vector3 lightDirection = m_skyboxRotation.Rotate(Vector3(0.2f, 0.0f, 0.5f));
		lightDirection.Normalize();

		Colour fogColour = FOG_COLOUR_SUNRISE.Interpolate(FOG_COLOUR_NIGHT, GetSunriseFactor());
		fogColour = fogColour.Interpolate(FOG_COLOUR_DAY, GetShadowFactor());

		m_sunPosition = lightDirection * Vector3(-6048.0f, -6048.0f, -6048.0f);
		m_moonPosition = lightDirection * Vector3(6048.0f, 6048.0f, 6048.0f);

		/*if (Scenes::Get()->GetCamera() != nullptr)
		{
			m_sunPosition += Scenes::Get()->GetCamera()->GetPosition();
			m_moonPosition += Scenes::Get()->GetCamera()->GetPosition();
		}*/

		m_sunColour = SUN_COLOUR_SUNRISE.Interpolate(SUN_COLOUR_NIGHT, GetSunriseFactor());
		m_sunColour = m_sunColour.Interpolate(SUN_COLOUR_DAY, GetShadowFactor());

		m_moonColour = MOON_COLOUR_NIGHT.Interpolate(MOON_COLOUR_DAY, GetShadowFactor());

		m_fog.SetColour(fogColour);
		m_fog.SetDensity(0.002f + ((1.0f - GetShadowFactor()) * 0.002f));
		m_fog.SetGradient(2.0f - ((1.0f - GetShadowFactor()) * 0.380f));
		m_fog.SetLowerLimit(0.0f);
		m_fog.SetUpperLimit(0.15f - ((1.0f - GetShadowFactor()) * 0.03f));

		m_skyColour = SKYBOX_COLOUR_DAY;

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->SetLightDirection(lightDirection);
			Shadows::Get()->SetShadowBoxOffset((4.0f * (1.0f - GetShadowFactor())) + 10.0f);
			Shadows::Get()->SetShadowBoxDistance(40.0f);
			Shadows::Get()->SetShadowTransition(5.0f);
			Shadows::Get()->SetShadowDarkness(0.6f * GetShadowFactor());
		}
	}

	float MainWorld::GetDayFactor() const
	{
		return m_factorDay;
	}

	float MainWorld::GetSunriseFactor() const
	{
		return Maths::Clamp(-(std::sin(2.0f * PI * GetDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
	}

	float MainWorld::GetShadowFactor() const
	{
		return Maths::Clamp(1.7f * std::sin(2.0f * PI * GetDayFactor()), 0.0f, 1.0f);
	}

	float MainWorld::GetSunHeight() const
	{
		return m_sunPosition.m_y;
	}

	float MainWorld::GetStarIntensity() const
	{
		return Maths::Clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
	}

	float MainWorld::GetTerrainRadius(const float &radius, const float &theta, const float &phi)
	{
		float height = m_noiseTerrain.GetValue(
			(radius / 10.0f) * Maths::NormalizeAngle(Maths::Degrees(theta)),
			(radius / 10.0f) * Maths::NormalizeAngle(Maths::Degrees(phi))
		);
		return radius + (28.0f * height);
	}
}
