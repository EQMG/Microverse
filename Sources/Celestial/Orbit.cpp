#include "Orbit.hpp"

#include <Engine/Log.hpp>
#include <Maths/Maths.hpp>
#include <Objects/GameObject.hpp>
#include "Star.hpp"

namespace micro
{
	Orbit::Orbit(const float &eccentricity, const float &semiMajorAxis, const float &parentMass, const float &pitch, const float &yaw, const float &roll) :
		m_eccentricity(eccentricity),
		m_semiMajorAxis(semiMajorAxis),
		m_semiMinorAxis(semiMajorAxis * std::sqrt(1.0f - std::pow(eccentricity, 2.0f))),
		m_periapsis(semiMajorAxis * std::sqrt(1 - eccentricity)),
		m_apoapsis(semiMajorAxis * std::sqrt(1 + eccentricity)),
		m_period(std::sqrt(std::pow(semiMajorAxis, 3.0f) / (parentMass / Star::MEDIAN_MASS)) / 365.25f),
		m_velocity(std::sqrt((parentMass / Star::MEDIAN_MASS) / semiMajorAxis) * 29780.0f),
		m_pitch(pitch),
		m_yaw(yaw),
		m_roll(roll)
	{
		Log::Out("Orbit: Eccentricity=%f, Semimajor Axis (AU)=%f, Semiminor Axis (AU)=%f, Periapsis(AU)=%f, Apoapsis(AU)=%f, Period(Days)=%f, "
				  "Velocity(m/s)=%f, Pitch(Deg)=%f, Yaw(Deg)=%f, Roll(Deg)=%f\n",
			m_eccentricity, m_semiMajorAxis, m_semiMinorAxis, m_periapsis, m_apoapsis, m_period,
			m_velocity, m_pitch, m_yaw, m_roll);
	}

	void Orbit::Start()
	{
	}

	void Orbit::Update()
	{
	//	auto parent = GetGameObject()->GetParent();

	//	if (parent == nullptr)
	//	{
	//		return;
	//	}

	//	auto celestial = GetGameObject()->GetComponent<ICelestial>();
	//	auto parentCelestial = GetGameObject()->GetParent()->GetComponent<ICelestial>();

	//	Vector3 position = GetGameObject()->GetTransform().GetPosition();
	//	Vector3 parentPosition = GetGameObject()->GetParent()->GetTransform().GetPosition();
	//	float force = (Star::G_CONSTANT * parentCelestial->GetMass() * celestial->GetMass()) / parentPosition.DistanceSquared(position);
	//	GetGameObject()->GetTransform().SetPosition(position + (Vector3::FRONT * force));
	}

	void Orbit::Decode(const Metadata &metadata)
	{
	}

	void Orbit::Encode(Metadata &metadata) const
	{
	}
}
