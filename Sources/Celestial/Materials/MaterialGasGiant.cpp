#include "MaterialGasGiant.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
	MaterialGasGiant::MaterialGasGiant(const float &hueOffset, const float &timeScale) :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/GasGiants/GasGiant.vert", "Shaders/GasGiants/GasGiant.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {})),
		m_bandLookup(Texture::Resource("BandLookup.png")),
		m_hueOffset(hueOffset),
		m_timeScale(timeScale),
		m_octaves(5)
	{
	}

	MaterialGasGiant::~MaterialGasGiant()
	{
	}

	void MaterialGasGiant::Start()
	{
	}

	void MaterialGasGiant::Update()
	{
		Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
		Vector3 planetPosition = GetGameObject()->GetTransform().GetPosition();
		float distance = planetPosition.Distance(cameraPosition);
		m_octaves = static_cast<int>(Maths::Clamp((-5.312e-4f * distance) + 6.204f, 1.0f, 6.0f));
	}

	void MaterialGasGiant::Load(LoadedValue *value)
	{
	}

	void MaterialGasGiant::Write(LoadedValue *destination)
	{
	}

	void MaterialGasGiant::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("hueOffset", m_hueOffset);
		uniformObject.Push("time", Engine::Get()->GetTime() * m_timeScale);
		uniformObject.Push("octaves", m_octaves);
	}

	void MaterialGasGiant::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerBandLookup", m_bandLookup);
	}
}
