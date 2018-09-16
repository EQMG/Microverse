#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace micro
{
	class MaterialFog :
			public IMaterial
	{
	private:
		float m_thickness;
		float m_innerRadius;
		float m_outerRadius;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialFog(const float &thickness = 1.3f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<PipelineMaterial> GetMaterialPipeline() const override { return m_material; }
	};
}
