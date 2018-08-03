#include "Gravity.hpp"
#include "Planet.hpp"

#include <Objects/GameObject.hpp>
#include <Physics/Rigidbody.hpp>

namespace test
{
	Gravity::Gravity() :
		IComponent(),
		m_force(nullptr)
	{
	}

	Gravity::~Gravity()
	{
	}

	void Gravity::Start()
	{
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (rigidbody == nullptr)
		{
			fprintf(stderr, "Gravity must be attached to a object with a rigidbody!");
			return;
		}

		m_force = rigidbody->AddForce<Force>(Vector3(), Vector3::ZERO);
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
		float planetMass = 157079637590016.0f;

		float force = (Planet::G_CONSTANT * rigidbody->GetMass() * planetMass) / planet.DistanceSquared(position);
		m_force->SetForce(force * (planet - position));
	}

	void Gravity::Load(LoadedValue *value)
	{
	}

	void Gravity::Write(LoadedValue *destination)
	{
	}
}
