#pragma once

#include <Materials/IMaterial.hpp>

using namespace fl;

namespace test
{
	/// <summary>
	/// Class that represents a terrain material shader.
	/// </summary>
	class MaterialTerrain :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialTerrain();

		~MaterialTerrain();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::string GetName() const override { return "MaterialTerrain"; };

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
