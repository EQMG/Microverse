#include "Planet.hpp"

#include <Objects/GameObject.hpp>
#include <Physics/Rigidbody.hpp>
#include "Gravity.hpp"
#include "Star.hpp"

namespace test
{
	Gravity::Gravity() :
		IComponent()
	{
	}

	Gravity::~Gravity()
	{
	}

	void Gravity::Start()
	{
	}

	void Gravity::Update()
	{
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (rigidbody == nullptr)
		{
			return;
		}

		Vector3 position = GetGameObject()->GetTransform().GetPosition();
		Vector3 planet = Vector3();
		float planetMass = 159467253530624.000000f;

		float force = (Star::G_CONSTANT * rigidbody->GetMass() * planetMass) / planet.DistanceSquared(position);
		rigidbody->SetGravity(force * (planet - position).Normalize());
	}

	void Gravity::Load(LoadedValue *value)
	{
	}

	void Gravity::Write(LoadedValue *destination)
	{
	}
}
