#pragma once

#include <Materials/Material.hpp>

using namespace acid;

namespace micro
{
	class MaterialChunk :
		public Material
	{
	public:
		MaterialChunk();

		void Start() override;

		void Update() override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		friend const Metadata &operator>>(const Metadata &metadata, MaterialChunk &material);

		friend Metadata &operator<<(Metadata &metadata, const MaterialChunk &material);
	};
}
