#include "MaterialFog.hpp"

#include <Objects/GameObject.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Models/VertexModel.hpp>
#include <Meshes/Mesh.hpp>
#include <Scenes/Scenes.hpp>
#include "Celestial/ICelestial.hpp"

namespace test
{
	MaterialFog::MaterialFog(const float &thickness) :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 1}, PipelineCreate({"Shaders/Fogs/Fog.vert", "Shaders/Fogs/Fog.frag"},
		    VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {}))),
		m_thickness(thickness),
		m_innerRadius(0.0f),
		m_outerRadius(0.0f)
	{
	}

	MaterialFog::~MaterialFog()
	{
	}

	void MaterialFog::Start()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent != nullptr)
		{
			auto celestial = parent->GetComponent<ICelestial>(true);

			if (celestial != nullptr)
			{
				m_innerRadius = celestial->GetRadius();
				m_outerRadius = celestial->GetRadius() * m_thickness;
			}
		}

		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr)
		{
			mesh->SetModel(ModelSphere::Resource(30, 30, m_outerRadius));
		}
	}

	void MaterialFog::Update()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent == nullptr)
		{
			return;
		}

		GetGameObject()->GetTransform().SetPosition(parent->GetTransform().GetPosition());
	}

	void MaterialFog::Load(LoadedValue *value)
	{
	}

	void MaterialFog::Write(LoadedValue *destination)
	{
	}

	void MaterialFog::PushUniforms(UniformHandler &uniformObject)
	{
		Vector3 lightPos = Vector3(0.0f, 0.0f, 8000.0f);

		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("colourCeiling", Colour::WHITE);
		uniformObject.Push("colourFloor", Colour::WHITE);
		uniformObject.Push("colourNight", Colour("#3B445B"));
		uniformObject.Push("planetPos", GetGameObject()->GetTransform().GetPosition());
		uniformObject.Push("lightPos", lightPos);
		uniformObject.Push("innerRadius", m_innerRadius);
		uniformObject.Push("outerRadius", m_outerRadius);
	}

	void MaterialFog::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
