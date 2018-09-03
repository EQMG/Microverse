#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace micro
{
	class MaterialRing :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
		std::shared_ptr<Texture> m_ringLookup;
		float m_innerScale;
		float m_outerScale;
		float m_planetRadius;
		float m_innerRadius;
		float m_outerRadius;
	public:
		MaterialRing(const float &innerScale = 1.34f, const float &outerScale = 2.44f);

		~MaterialRing();

		void Start() override;

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
