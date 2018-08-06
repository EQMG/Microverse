#include "MaterialGasGiant.hpp"

#include <Maths/Maths.hpp>
#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
	MaterialGasGiant::MaterialGasGiant(const std::shared_ptr<Texture> &bandLookup, const float &hueOffset, const float &timeScale) :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/GasGiants/GasGiant.vert", "Shaders/GasGiants/GasGiant.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK, {}))),
		m_bandLookup(bandLookup),
		m_hueOffset(hueOffset),
		m_timeScale(timeScale),
		m_diffuseCompute(Compute(ComputeCreate("Shaders/GasGiants/GasGiant.comp", 3072, 3072, 32, {}))),
		m_diffuseTexture(std::make_shared<Texture>(3072, 3072)),
		m_diffuseUpdate(Timer(Maths::Random(16.0f, 20.0f)))
	{
		UpdateDiffuse();
	}

	MaterialGasGiant::~MaterialGasGiant()
	{
	}

	void MaterialGasGiant::Start()
	{
	}

	void MaterialGasGiant::Update()
	{
		if (m_diffuseUpdate.IsPassedTime())
		{
			m_diffuseUpdate.ResetStartTime();
			UpdateDiffuse();
		}
	}

	void MaterialGasGiant::Load(LoadedValue *value)
	{
		m_hueOffset = value->GetChild("Hue Offset")->Get<float>();
		m_timeScale = value->GetChild("Time Scale")->Get<float>();
	}

	void MaterialGasGiant::Write(LoadedValue *destination)
	{
		destination->GetChild("Hue Offset", true)->Set(m_hueOffset);
		destination->GetChild("Time Scale", true)->Set(m_timeScale);
	}

	void MaterialGasGiant::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
	}

	void MaterialGasGiant::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerDiffuse", m_diffuseTexture);
	}

	void MaterialGasGiant::UpdateDiffuse()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif
		CommandBuffer commandBuffer = CommandBuffer(true, COMMAND_BUFFER_LEVEL_PRIMARY, COMMAND_QUEUE_COMPUTE);

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
		fprintf(stdout, "Computed gas giant diffuse in %fms\n", debugEnd - debugStart);
#endif
	}
}
