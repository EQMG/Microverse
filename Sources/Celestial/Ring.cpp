#include "Ring.hpp"

#include <Models/Shapes/ModelDisk.hpp>
#include <Meshes/Mesh.hpp>

namespace test
{
	const float Ring::INNER_SCALE = 0.8f;
	const float Ring::OUTER_SCALE = 1.2f;

	Ring::Ring(const float &innerRadius, const float &outerRadius) :
		IComponent(),
		m_innerRadius(innerRadius),
		m_outerRadius(outerRadius)
	{
	}

	Ring::~Ring()
	{
	}

	void Ring::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr)
		{
			mesh->SetModel(ModelDisk::Resource(m_innerRadius * INNER_SCALE, m_outerRadius * OUTER_SCALE, 16, 2));
		}
	}

	void Ring::Update()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent == nullptr)
		{
			return;
		}

		GetGameObject()->GetTransform().SetPosition(parent->GetTransform().GetPosition());
	}

	void Ring::Load(LoadedValue *value)
	{
		m_innerRadius = value->GetChild("Inner Radius")->Get<float>();
		m_outerRadius = value->GetChild("Outer Radius")->Get<float>();
	}

	void Ring::Write(LoadedValue *destination)
	{
		destination->GetChild("Inner Radius", true)->Set(m_innerRadius);
		destination->GetChild("Outer Radius", true)->Set(m_outerRadius);
	}
}
