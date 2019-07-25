#include "Gravity.hpp"

#include <Scenes/Entity.hpp>
#include <Physics/Rigidbody.hpp>
#include <Scenes/Scenes.hpp>
#include "Star.hpp"

namespace micro
{
	Gravity::Gravity()
	{
	}

	void Gravity::Start()
	{
	}

	void Gravity::Update()
	{
		auto rigidbody{GetParent()->GetComponent<Rigidbody>()};

		if (rigidbody == nullptr)
		{
			return;
		}

		auto position{GetParent()->GetWorldTransform().GetPosition()};
		auto celestialList{Scenes::Get()->GetStructure()->QueryComponents<Celestial>()}; // TODO: Only re-query on component type insert.
		m_influence = nullptr;

		Vector3f forcesSum;
		Vector3f strongest;

		for (auto &celestial : celestialList)
		{
			auto celestialPosition{celestial->GetParent()->GetWorldTransform().GetPosition()};
			auto force{(Star::GConstant * rigidbody->GetMass() * celestial->GetMass()) / celestialPosition.DistanceSquared(position)};
			auto vector{force * (celestialPosition - position).Normalize()};

			if (vector.LengthSquared() > strongest.LengthSquared())
			{
				strongest = vector;
				m_influence = celestial;
			}

			forcesSum += vector;
		}

		rigidbody->SetGravity(forcesSum);
	}

	const Metadata &operator>>(const Metadata &metadata, Gravity &gravity)
	{
		return metadata;
	}

	Metadata &operator<<(Metadata &metadata, const Gravity &gravity)
	{
		return metadata;
	}
}
