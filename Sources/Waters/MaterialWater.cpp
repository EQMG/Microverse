#include "MaterialWater.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include "MeshWater.hpp"

namespace test
{
	MaterialWater::MaterialWater() :
		IMaterial(),
		m_colour(Colour(MeshWater::WATER_COLOUR)),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Waters/Water.vert", "Shaders/Waters/Water.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {})))
	{
	}

	MaterialWater::~MaterialWater()
	{
	}

	void MaterialWater::Start()
	{
		m_colour.m_a = 1.0f; // Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f
	}

	void MaterialWater::Update()
	{
	}

	void MaterialWater::Load(LoadedValue *value)
	{
	}

	void MaterialWater::Write(LoadedValue *destination)
	{
	}

	void MaterialWater::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("diffuseColour", m_colour);
	}

	void MaterialWater::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
