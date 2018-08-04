#include "Planet.hpp"

#include <Objects/GameObject.hpp>
#include <Physics/Rigidbody.hpp>
#include <Scenes/Scenes.hpp>
#include "Gravity.hpp"
#include "Star.hpp"

namespace test
{
	Gravity::Gravity() :
		IComponent(),
		m_influence(nullptr)
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
		auto celestialList = Scenes::Get()->GetStructure()->QueryComponents<ICelestial>(); // TODO: Only select on component type insert.
		m_influence = nullptr;

		Vector3 forcesSum = Vector3();
		Vector3 strongest = Vector3();

		for (auto &celestial : celestialList)
		{
			Vector3 celestialPosition = celestial->GetGameObject()->GetTransform().GetPosition();
			float force = (Star::G_CONSTANT * rigidbody->GetMass() * celestial->GetMass()) / celestialPosition.DistanceSquared(position);
			Vector3 vector = force * (celestialPosition - position).Normalize();

			if (vector.LengthSquared() > strongest.LengthSquared())
			{
				strongest = vector;
				m_influence = celestial;
			}

			forcesSum += vector;
		}

		rigidbody->SetGravity(forcesSum);
	}

	void Gravity::Load(LoadedValue *value)
	{
	}

	void Gravity::Write(LoadedValue *destination)
	{
	}
}
