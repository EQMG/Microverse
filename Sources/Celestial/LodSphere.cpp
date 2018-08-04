#include "LodSphere.hpp"

#include <Scenes/Scenes.hpp>
#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Events/EventTime.hpp>
#include <Events/Events.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include "ICelestial.hpp"

namespace test
{
	const uint32_t LodSphere::HIGHEST_LOD = 2;

	LodSphere::LodSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius) :
		IComponent(),
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
		uint32_t latitudeBands = m_latitudeBands;
		uint32_t longitudeBands = m_longitudeBands;

		for (int i = HIGHEST_LOD - 1; i >= 0; i--)
		{
			m_lods[i] = ModelSphere::Resource(latitudeBands, longitudeBands, m_radius);
			latitudeBands = static_cast<uint32_t>(std::floor(static_cast<float>(latitudeBands) / 2.0f));
			longitudeBands = static_cast<uint32_t>(std::floor(static_cast<float>(longitudeBands) / 2.0f));
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
		m_latitudeBands = value->GetChild("Latitude Bands")->Get<uint32_t>();
		m_longitudeBands = value->GetChild("Longitude Bands")->Get<uint32_t>();
		m_radius = value->GetChild("Radius")->Get<float>();
	}

	void LodSphere::Write(LoadedValue *destination)
	{
		destination->GetChild("Latitude Bands", true)->Set(m_latitudeBands);
		destination->GetChild("Longitude Bands", true)->Set(m_longitudeBands);
		destination->GetChild("Radius", true)->Set(m_radius);
	}

	uint32_t LodSphere::CalculateLod()
	{
		Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
		Vector3 entityPosition = GetGameObject()->GetTransform().GetPosition();

		auto celestial = GetGameObject()->GetComponent<ICelestial>();
		float radius = celestial == nullptr ? 0.0f : celestial->GetRadius();

		float distance = std::fabs(entityPosition.Distance(cameraPosition) - radius);
		float lod = std::floor((-0.3f / radius) * distance + HIGHEST_LOD);
		return static_cast<uint32_t>(Maths::Clamp(lod, 0.0f, HIGHEST_LOD - 1));
	}
}
