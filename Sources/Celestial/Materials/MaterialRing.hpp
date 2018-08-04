#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class MaterialRing :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
		std::shared_ptr<Texture> m_ringLookup;
		float m_innerRadius;
		float m_outerRadius;
	public:
		MaterialRing();

		~MaterialRing();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::string GetName() const override { return "MaterialRing"; };

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
