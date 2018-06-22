#include "MaterialTerrain.hpp"

namespace test
{
	MaterialTerrain::MaterialTerrain() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_LINE, PIPELINE_CULL_MODE_NONE), {}))
	{
	}

	MaterialTerrain::~MaterialTerrain()
	{
	}

	void MaterialTerrain::Update()
	{
	}

	void MaterialTerrain::Load(std::shared_ptr<LoadedValue> value)
	{
	}

	void MaterialTerrain::Write(std::shared_ptr<LoadedValue> destination)
	{
	}

	void MaterialTerrain::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
	}

	void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
