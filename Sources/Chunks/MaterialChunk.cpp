#include "MaterialChunk.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include <Celestial/Planet.hpp>

namespace micro
{
	MaterialChunk::MaterialChunk() :
		IMaterial(),
		m_material(nullptr)
	{
	}

	void MaterialChunk::Start()
	{
		m_material = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Chunks/Chunk.vert", "Shaders/Chunks/Chunk.frag"},
			{VertexModel::GetVertexInput()}, PIPELINE_MODE_MRT, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}));
	}

	void MaterialChunk::Update()
	{
	}

	void MaterialChunk::Decode(const Metadata &metadata)
	{
	}

	void MaterialChunk::Encode(Metadata &metadata) const
	{
	}

	void MaterialChunk::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
	}

	void MaterialChunk::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("test", GetGameObject()->GetParent()->GetComponent<Planet>()->test);
	}
}
