#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>

using namespace acid;

namespace test
{
	/// <summary>
	/// Class that represents a terrain material shader.
	/// </summary>
	class MaterialTerrain :
		public IMaterial
	{
	private:
		Colour m_baseColor;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialTerrain(const Colour &baseColor = Colour::WHITE);

		~MaterialTerrain();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::string GetName() const override { return "MaterialTerrain"; };

		Colour GetBaseColor() const { return m_baseColor; }

		void SetBaseColor(const Colour &baseColor) { m_baseColor = baseColor; }

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
