#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>
#include <Renderer/Pipelines/Compute.hpp>

using namespace acid;

namespace micro
{
	class MaterialGasGiant :
		public IMaterial
	{
	private:
		std::shared_ptr<Texture> m_bandLookup;
		float m_radius;
		float m_hueOffset;
		float m_timeScale;

		Compute m_diffuseCompute;
		std::shared_ptr<Texture> m_diffuseTexture;
		Timer m_diffuseUpdate;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialGasGiant(const std::shared_ptr<Texture> &bandLookup = nullptr, const float &hueOffset = 0.0f, const float &timeScale = 0.00002f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<PipelineMaterial> GetMaterialPipeline() const override { return m_material; }

		float GetHueOffset() const { return m_hueOffset; }

		void SetHueOffset(const float &hueOffset) { m_hueOffset = hueOffset; }

		float GetTimeScale() const { return m_timeScale; }

		void SetTimeScale(const float &timeScale) { m_timeScale = timeScale; }
	private:
		void UpdateDiffuse();

		void WriteDiffuse();
	};
}
