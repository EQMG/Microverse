#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>

using namespace acid;

namespace micro
{
	class MaterialChunk :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialChunk();

		~MaterialChunk();

		void Start() override;

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
