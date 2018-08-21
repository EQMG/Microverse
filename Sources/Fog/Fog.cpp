#include "Fog.hpp"

#include <Models/Shapes/ModelSphere.hpp>
#include <Models/Obj/ModelObj.hpp>
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
		m_innerRadius(700.0f),
		m_outerRadius(1000.0f)
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
			float radius = celestial->GetRadius();
			m_innerRadius = radius;
			m_outerRadius = radius * m_thickness;
		}

	//	m_sphere = ModelSphere::Resource(20, 20, m_outerRadius);
		m_sphere = ModelObj::Resource("Icosphere.obj");
	}

	void Fog::Update()
	{
		auto camera = Scenes::Get()->GetCamera();
		auto transform = GetGameObject()->GetTransform();
		transform.SetScaling(m_outerRadius * Vector3::ONE);

		Vector3 lightPos = Vector3(0.0f, 0.0f, 8000.0f);
		Vector3 planetToCamera = camera->GetPosition() - transform.GetPosition();
		Colour wavelength = Colour(std::pow(0.65f, 4.0f), std::pow(0.57f, 4.0f), std::pow(0.475f, 4.0f));

		float gMie = -0.95f;
		float KrESun = 0.0025f * 15.0f;
		float KmESun = 0.0015f * 15.0f;
		float Kr4PI = 0.0025f * 4.0f * PI;
		float Km4PI = 0.0015f * 4.0f * PI;

		float scale = 1.0f / (m_outerRadius - m_innerRadius);
		float scaleDepth = 0.25f;
		float scaleOverScaleDepth = scale / scaleDepth;
		float samples = 4.0f;

		float hdrExposure = 0.9f;

		m_uniformObject.Push("transform", transform.GetWorldMatrix());
		m_uniformObject.Push("planetPos", transform.GetPosition());
		m_uniformObject.Push("lightPos", lightPos);
		m_uniformObject.Push("invWavelength", 1.0f / wavelength);
		m_uniformObject.Push("innerRadius", m_innerRadius);
		m_uniformObject.Push("outerRadius", m_outerRadius);
		m_uniformObject.Push("gMie", gMie);
		m_uniformObject.Push("KrESun", KrESun);
		m_uniformObject.Push("KmESun", KmESun);
		m_uniformObject.Push("Kr4PI", Kr4PI);
		m_uniformObject.Push("Km4PI", Km4PI);
		m_uniformObject.Push("scale", scale);
		m_uniformObject.Push("scaleDepth", scaleDepth);
		m_uniformObject.Push("scaleOverScaleDepth", scaleOverScaleDepth);
		m_uniformObject.Push("samples", samples);
		m_uniformObject.Push("hdrExposure", hdrExposure);
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

	bool Fog::operator<(const Fog &other) const
	{
		auto camera = Scenes::Get()->GetCamera();

		float thisDistance2 = (camera->GetPosition() - GetGameObject()->GetTransform().GetPosition()).LengthSquared();
		float otherDistance2 = (camera->GetPosition() - other.GetGameObject()->GetTransform().GetPosition()).LengthSquared();

		return thisDistance2 > otherDistance2;
	}
}
