#include "MaterialRing.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include <Scenes/Scenes.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/ModelDisk.hpp>
#include "Celestial/ICelestial.hpp"

namespace test
{
	MaterialRing::MaterialRing(const float &innerScale, const float &outerScale) :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 1}, PipelineCreate({"Shaders/Rings/Ring.vert", "Shaders/Rings/Ring.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, {}))),
		m_ringLookup(Texture::Resource("RingLookup.png")),
		m_innerScale(innerScale),
		m_outerScale(outerScale),
		m_innerRadius(1000.0f),
		m_outerRadius(1300.0f)
	{
	}

	MaterialRing::~MaterialRing()
	{
	}

	void MaterialRing::Start()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent != nullptr)
		{
			auto celestial = parent->GetComponent<ICelestial>(true);

			if (celestial != nullptr)
			{
				m_innerRadius = celestial->GetRadius() * m_innerScale;
				m_outerRadius = celestial->GetRadius() * m_outerScale;
			}
		}

		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr)
		{
			mesh->SetModel(ModelDisk::Resource(m_innerRadius * 0.8f, m_outerRadius * 1.2f, 16, 2));
		}
	}

	void MaterialRing::Update()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent == nullptr)
		{
			return;
		}

		GetGameObject()->GetTransform().SetPosition(parent->GetTransform().GetPosition());
	}

	void MaterialRing::Load(LoadedValue *value)
	{
	}

	void MaterialRing::Write(LoadedValue *destination)
	{
	}

	void MaterialRing::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("innerRadius", m_innerRadius);
		uniformObject.Push("outerRadius", m_outerRadius);
	}

	void MaterialRing::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerRingLookup", m_ringLookup);
	}
}
