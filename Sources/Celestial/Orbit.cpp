#include "Orbit.hpp"
#include "Star.hpp"

#include <Objects/GameObject.hpp>

namespace test
{
	Orbit::Orbit() :
		IComponent()
	{
	}

	Orbit::~Orbit()
	{
	}

	void Orbit::Start()
	{
	}

	void Orbit::Update()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent == nullptr)
		{
			return;
		}

		auto celestial = GetGameObject()->GetComponent<ICelestial>();
		auto parentCelestial = GetGameObject()->GetParent()->GetComponent<ICelestial>();

	//	Vector3 position = GetGameObject()->GetTransform().GetPosition();
	//	Vector3 parentPosition = GetGameObject()->GetParent()->GetTransform().GetPosition();
	//	float force = (Star::G_CONSTANT * parentCelestial->GetMass() * celestial->GetMass()) / parentPosition.DistanceSquared(position);
	//	GetGameObject()->GetTransform().SetPosition(position + (Vector3::FRONT * force));
	}

	void Orbit::Load(LoadedValue *value)
	{
		// TODO
	}

	void Orbit::Write(LoadedValue *destination)
	{
	}
}
