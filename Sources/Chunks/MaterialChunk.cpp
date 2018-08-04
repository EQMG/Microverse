#include "MaterialChunk.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialChunk::MaterialChunk() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Chunks/Chunk.vert", "Shaders/Chunks/Chunk.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {}))
	{
	}

	MaterialChunk::~MaterialChunk()
	{
	}

	void MaterialChunk::Start()
	{
	}

	void MaterialChunk::Update()
	{
	}

	void MaterialChunk::Load(LoadedValue *value)
	{
	}

	void MaterialChunk::Write(LoadedValue *destination)
	{
	}

	void MaterialChunk::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
	}

	void MaterialChunk::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
