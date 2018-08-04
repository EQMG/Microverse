#include "Ring.hpp"

#include <Models/Shapes/ModelDisk.hpp>
#include <Meshes/Mesh.hpp>

namespace test
{
	const float Ring::INNER_SCALE = 0.8f;
	const float Ring::OUTER_SCALE = 1.2f;

	Ring::Ring(ICelestial *parent, const float &innerRadius, const float &outerRadius) :
		IComponent(),
		m_parent(parent),
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
		GetGameObject()->SetTransform(m_parent->GetGameObject()->GetTransform());
	}

	void Ring::Load(LoadedValue *value)
	{
	}

	void Ring::Write(LoadedValue *destination)
	{
	}
}
