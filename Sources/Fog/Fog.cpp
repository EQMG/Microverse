#include "Fog.hpp"

#include <Models/Shapes/ModelSphere.hpp>
#include <Objects/GameObject.hpp>
#include <Scenes/Scenes.hpp>
#include <Maths/Colour.hpp>
#include "Celestial/ICelestial.hpp"

namespace test
{
	Fog::Fog(const float &thickness) :
		IComponent(),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler()),
		m_sphere(nullptr),
		m_thickness(thickness),
		m_radius(1000.0f),
		m_planetToCamera(Vector3())
	{
	}

	Fog::~Fog()
	{
	}

	void Fog::Start()
	{
		auto celestial = GetGameObject()->GetComponent<ICelestial>();

		if (celestial != nullptr)
		{
			m_radius = celestial->GetRadius();
		}

		m_sphere = ModelSphere::Resource(30, 30, m_radius * m_thickness);
	}

	void Fog::Update()
	{
		auto camera = Scenes::Get()->GetCamera();
		auto transform = GetGameObject()->GetTransform();

		Vector3 lightDirection = Vector3(0.0f, 0.0f, 8000.0f) - transform.GetPosition();
		m_planetToCamera = camera->GetPosition() - transform.GetPosition();
		Vector3 wavelength = Vector3(std::pow(0.65f, 4.0f), std::pow(0.57f, 4.0f), std::pow(0.475f, 4.0f));

		m_uniformObject.Push("transform", transform.GetWorldMatrix());
		m_uniformObject.Push("cameraPos", m_planetToCamera);
		m_uniformObject.Push("lightDir", lightDirection.Normalize());
		m_uniformObject.Push("invWavelength", 1.0f / wavelength);
		m_uniformObject.Push("cameraHeight", m_planetToCamera.Length());
		m_uniformObject.Push("innerRadius", m_radius);
		m_uniformObject.Push("outerRadius", m_radius * m_thickness);
		m_uniformObject.Push("scaleDepth", 0.25f);
	}

	void Fog::Load(LoadedValue *value)
	{
	}

	void Fog::Write(LoadedValue *destination)
	{
	}

	void Fog::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline &pipeline)
	{
		// Binds the material pipeline.
		pipeline.BindPipeline(commandBuffer);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		m_sphere->CmdRender(commandBuffer);
	}
}
