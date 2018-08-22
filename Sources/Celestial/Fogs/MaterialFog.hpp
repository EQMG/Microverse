#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class MaterialFog :
			public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
		float m_thickness;
		float m_innerRadius;
		float m_outerRadius;
	public:
		MaterialFog(const float &thickness = 1.41f);

		~MaterialFog();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
