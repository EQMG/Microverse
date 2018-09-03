#include "MaterialFog.hpp"

#include <Objects/GameObject.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Models/VertexModel.hpp>
#include <Meshes/Mesh.hpp>
#include <Scenes/Scenes.hpp>
#include <Models/Obj/ModelObj.hpp>
#include "Celestial/ICelestial.hpp"

namespace micro
{
	MaterialFog::MaterialFog(const float &thickness) :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 1}, PipelineCreate({"Shaders/Fogs/Fog.vert", "Shaders/Fogs/Fog.frag"},
		    VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {}))),
		m_thickness(thickness),
		m_innerRadius(0.0f),
		m_outerRadius(0.0f)
	{
	}

	MaterialFog::~MaterialFog()
	{
	}

	void MaterialFog::Start()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent != nullptr)
		{
			auto celestial = parent->GetComponent<ICelestial>(true);

			if (celestial != nullptr)
			{
				m_innerRadius = celestial->GetRadius();
				m_outerRadius = celestial->GetRadius() * m_thickness;
			}
		}

		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr)
		{
			mesh->SetModel(ModelObj::Resource("Icosphere.obj"));
		//	mesh->SetModel(ModelSphere::Resource(20, 20, m_outerRadius)); //  * 1.2f
		}
	}

	void MaterialFog::Update()
	{
		auto parent = GetGameObject()->GetParent();

		if (parent == nullptr)
		{
			return;
		}

		GetGameObject()->GetTransform().SetPosition(parent->GetTransform().GetPosition());
	}

	void MaterialFog::Decode(const Node &node)
	{
	}

	void MaterialFog::Encode(Node &node) const
	{
	}

	void MaterialFog::PushUniforms(UniformHandler &uniformObject)
	{
		Transform transform = GetGameObject()->GetTransform();
		transform.SetScaling(Vector3(m_outerRadius, m_outerRadius, m_outerRadius));

		Vector3 lightPos = Vector3(0.0f, 0.0f, 0.0f);

		Colour wavelength = Colour(std::pow(0.65f, 4.0f), std::pow(0.57f, 4.0f), std::pow(0.475f, 4.0f));
		float gMie = -0.95f;
		float gMie2 = gMie * gMie;
		float KrESun = 0.0025f * 15.0f;
		float KmESun = 0.0015f * 15.0f;
		float Kr4PI = 0.0025f * 4.0f * PI;
		float Km4PI = 0.0015f * 4.0f * PI;

		float scale = 1.0f / (m_outerRadius - m_innerRadius);
		float scaleDepth = 0.25f;
		float scaleOverScaleDepth = scale / scaleDepth;
		float samples = 2.0f;

		float hdrExposure = 0.9f;

		uniformObject.Push("transform", transform.GetWorldMatrix());
		uniformObject.Push("planetPos", transform.GetPosition());
		uniformObject.Push("lightPos", lightPos);
		uniformObject.Push("invWavelength", 1.0f / wavelength);
		uniformObject.Push("innerRadius", m_innerRadius);
		uniformObject.Push("outerRadius", m_outerRadius);
		uniformObject.Push("gMie", gMie);
		uniformObject.Push("gMie2", gMie2);
		uniformObject.Push("KrESun", KrESun);
		uniformObject.Push("KmESun", KmESun);
		uniformObject.Push("Kr4PI", Kr4PI);
		uniformObject.Push("Km4PI", Km4PI);
		uniformObject.Push("scale", scale);
		uniformObject.Push("scaleDepth", scaleDepth);
		uniformObject.Push("scaleOverScaleDepth", scaleOverScaleDepth);
		uniformObject.Push("samples", samples);
		uniformObject.Push("hdrExposure", hdrExposure);
	}

	void MaterialFog::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
