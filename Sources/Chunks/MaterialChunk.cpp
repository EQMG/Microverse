#include "MaterialChunk.hpp"

#include <Scenes/Entity.hpp>
#include <Models/VertexDefault.hpp>
#include "Celestial/Planet.hpp"

namespace micro
{
	MaterialChunk::MaterialChunk()
	{
	}

	void MaterialChunk::Start()
	{
		m_pipelineMaterial = PipelineMaterial::Create({1, 0}, {{"Shaders/Chunks/Chunk.vert", "Shaders/Chunks/Chunk.frag"}, {VertexDefault::GetVertexInput()}, {}, 
			PipelineGraphics::Mode::Mrt, PipelineGraphics::Depth::ReadWrite, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT});
	}

	void MaterialChunk::Update()
	{
	}

	void MaterialChunk::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	}

	void MaterialChunk::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}

	const Metadata &operator>>(const Metadata &metadata, MaterialChunk &material)
	{
		return metadata;
	}

	Metadata &operator<<(Metadata &metadata, const MaterialChunk &material)
	{;
		return metadata;
	}
}
