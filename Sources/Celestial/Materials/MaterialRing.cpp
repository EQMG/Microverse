#include "MaterialRing.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include <Scenes/Scenes.hpp>
#include "Celestial/Ring.hpp"

namespace test
{
	MaterialRing::MaterialRing() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Rings/Ring.vert", "Shaders/Rings/Ring.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_NONE, {}))),
		m_ringLookup(Texture::Resource("RingLookup.png")),
		m_innerRadius(0.0f),
		m_outerRadius(0.0f)
	{
	}

	MaterialRing::~MaterialRing()
	{
	}

	void MaterialRing::Start()
	{
	}

	void MaterialRing::Update()
	{
		auto ring = GetGameObject()->GetComponent<Ring>(true);

		if (ring != nullptr)
		{
			m_innerRadius = ring->GetInnerRadius();
			m_outerRadius = ring->GetOuterRadius();
		}
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
