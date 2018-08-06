#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>
#include <Renderer/Pipelines/Compute.hpp>

using namespace acid;

namespace test
{
	class MaterialGasGiant :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
		std::shared_ptr<Texture> m_bandLookup;
		float m_hueOffset;
		float m_timeScale;

		Compute m_diffuseCompute;
		std::shared_ptr<Texture> m_diffuseTexture;
		Timer m_diffuseUpdate;
	public:
		MaterialGasGiant(const std::shared_ptr<Texture> &bandLookup = nullptr, const float &hueOffset = 0.0f, const float &timeScale = 0.00003f);

		~MaterialGasGiant();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::string GetName() const override { return "MaterialGasGiant"; };

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }

		float GetHueOffset() const { return m_hueOffset; }

		void SetHueOffset(const float &hueOffset) { m_hueOffset = hueOffset; }

		float GetTimeScale() const { return m_timeScale; }

		void SetTimeScale(const float &timeScale) { m_timeScale = timeScale; }
	private:
		void UpdateDiffuse();
	};
}
