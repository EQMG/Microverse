#include "MaterialGasGiant.hpp"

#include <Maths/Maths.hpp>
#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include <Scenes/Scenes.hpp>
#include <Helpers/FileSystem.hpp>
#include "Celestial/Planet.hpp"

namespace micro
{
	MaterialGasGiant::MaterialGasGiant(const std::shared_ptr<Texture> &bandLookup, const float &hueOffset, const float &timeScale) :
		m_bandLookup(bandLookup),
		m_radius(0.0f),
		m_hueOffset(hueOffset),
		m_timeScale(timeScale),
		m_diffuseCompute(Compute(ComputeCreate("Shaders/GasGiants/GasGiant.comp", 3072, 3072, 32, {}))),
		m_diffuseTexture(std::make_shared<Texture>(3072, 3072)),
		m_diffuseUpdate(Timer(Maths::Random(16.0f, 20.0f))),
		m_material(nullptr)
	{
	}

	void MaterialGasGiant::Start()
	{
		m_material = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/GasGiants/GasGiant.vert", "Shaders/GasGiants/GasGiant.frag"},
			{VertexModel::GetVertexInput()}, PIPELINE_MODE_MRT, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}));

		auto planet = GetGameObject()->GetComponent<Planet>(true);

		if (planet != nullptr)
		{
			m_radius = planet->GetRadius();
		}

		UpdateDiffuse();
	//	WriteDiffuse();
	}

	void MaterialGasGiant::Update()
	{
		/*if (m_diffuseUpdate.IsPassedTime())
		{
			m_diffuseUpdate.ResetStartTime();
			UpdateDiffuse();
		}*/
	}

	void MaterialGasGiant::Decode(const Metadata &metadata)
	{
		m_hueOffset = metadata.GetChild<float>("Hue Offset");
		m_timeScale = metadata.GetChild<float>("Time Scale");
	}

	void MaterialGasGiant::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Hue Offset", m_hueOffset);
		metadata.SetChild("Time Scale", m_timeScale);
	}

	void MaterialGasGiant::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("position", GetGameObject()->GetTransform().GetPosition());
		uniformObject.Push("radius", m_radius);
	}

	void MaterialGasGiant::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerAlbedo", m_diffuseTexture);
	}

	void MaterialGasGiant::UpdateDiffuse()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);

		// Bind the pipeline.
		m_diffuseCompute.BindPipeline(commandBuffer);

		// Uniform Object.
		auto uniformBlock = m_diffuseCompute.GetShaderProgram()->GetUniformBlock("UboObject");
		UniformHandler uniform = UniformHandler(uniformBlock);
		uniform.Push("hueOffset", m_hueOffset);
		uniform.Push("time", Engine::Get()->GetTime() * m_timeScale);
		uniform.Push("octaves", 8);
		uniform.Update(uniformBlock);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(m_diffuseCompute);
		descriptorSet.Push("UboObject", uniform);
		descriptorSet.Push("samplerBandLookup", m_bandLookup);
		descriptorSet.Push("outColour", m_diffuseTexture);
		descriptorSet.Update(m_diffuseCompute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer);
		m_diffuseCompute.CmdRender(commandBuffer);

		commandBuffer.End();
		commandBuffer.Submit();
#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Computed gas giant diffuse in %fms\n", debugEnd - debugStart);
#endif
	}

	void MaterialGasGiant::WriteDiffuse()
	{
		std::string filename = FileSystem::GetWorkingDirectory() + "/" + GetGameObject()->GetName() + ".png";
		FileSystem::ClearFile(filename);
		uint8_t *pixels = m_diffuseTexture->GetPixels();
		Texture::WritePixels(filename, pixels, m_diffuseTexture->GetWidth(), m_diffuseTexture->GetHeight(), m_diffuseTexture->GetComponents());
		delete[] pixels;
	}
}
