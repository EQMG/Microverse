#include "LodSphere.hpp"

#include <Scenes/Scenes.hpp>
#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Events/EventTime.hpp>
#include <Events/Events.hpp>
#include <Models/Shapes/ModelSphere.hpp>

namespace test
{
	const uint32_t LodSphere::HIGHEST_LOD = 2;

	LodSphere::LodSphere(ICelestial *parent, const unsigned int &latitudeBands, const unsigned int &longitudeBands, const float &radius) :
		IComponent(),
		m_parent(parent),
		m_lods(std::vector<std::shared_ptr<Model>>(HIGHEST_LOD)),
		m_latitudeBands(latitudeBands),
		m_longitudeBands(longitudeBands),
		m_radius(radius)
	{
	}

	LodSphere::~LodSphere()
	{
	}

	void LodSphere::Start()
	{
		unsigned int latitudeBands = m_latitudeBands;
		unsigned int longitudeBands = m_longitudeBands;

		for (int i = HIGHEST_LOD - 1; i >= 0; i--)
		{
			m_lods[i] = ModelSphere::Resource(latitudeBands, longitudeBands, m_radius);
			latitudeBands = static_cast<unsigned int>(std::floor(static_cast<float>(latitudeBands) / 2.0f));
			longitudeBands = static_cast<unsigned int>(std::floor(static_cast<float>(longitudeBands) / 2.0f));
		}
	}

	void LodSphere::Update()
	{
		auto targetLod = CalculateLod();

		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr)
		{
			mesh->SetModel(m_lods[targetLod]);
		}
	}

	void LodSphere::Load(LoadedValue *value)
	{
	}

	void LodSphere::Write(LoadedValue *destination)
	{
	}

	uint32_t LodSphere::CalculateLod()
	{
		Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
		Vector3 entityPosition = GetGameObject()->GetTransform().GetPosition();
		float radius = m_parent->GetRadius();
		float distance = std::fabs(entityPosition.Distance(cameraPosition) - radius);
		float lod = std::floor((-0.3f / radius) * distance + HIGHEST_LOD);
		return static_cast<uint32_t>(Maths::Clamp(lod, 0.0f, HIGHEST_LOD - 1));
	}
}
