#include "Planet.hpp"

#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelRectangle.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"
#include "Star.hpp"

namespace micro
{
	const float Planet::MedianRadius = 700.0f; // +- 50.0%
	const float Planet::MedianDensity = 5510.0f;
	const float Planet::MedianMass = MedianMass * (4.0f / 3.0f) * Maths::Pi<float> * std::pow(MedianRadius, 3.0f);
	const float Planet::SquareRadiusRatio = 0.2f; // 0.2 or 0.4;

	Planet::Planet(const std::optional<uint64_t> &seed, const float &radius, const float &density, const float &axialTilt) :
		m_seed(seed),
		m_radius(radius),
		m_density(density),
		m_mass(m_density * (4.0f / 3.0f) *Maths::Pi<float> * std::pow(m_radius, 3.0f)),
		m_escapeVelocity(std::sqrt(2.0f * Star::GConstant * m_mass / m_radius)),
		m_axialTilt(axialTilt),
		m_axialTropics(axialTilt),
		m_axialPolar(90.0f - axialTilt),
		m_innerRings(1.34f * (m_radius / MedianRadius)),
		m_outterRings(2.44f * (m_radius / MedianRadius)),
		m_heightmapSize(3072)
	{
		auto surfaceGravity{Star::GConstant * m_mass / std::pow(m_radius, 2.0f)};

		Log::Out("Planet: Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Surface Gravity(m/s^2)=%f, Escape Velocity(m/s)=%f\n", m_radius, m_density, m_mass, surfaceGravity, m_escapeVelocity);
	}

	void Planet::Start()
	{
		if (!m_seed)
		{
			if (auto mesh = GetParent()->GetComponent<Mesh>(); mesh != nullptr)
			{
				mesh->SetModel(ModelSphere::Create(m_radius, 32, 32));
			//	mesh->SetModel(ModelObj::Resource("Icosphere.obj"));
			//	GetGameObject()->GetTransform().SetScaling(Vector3(m_radius, m_radius, m_radius));
			}

			return;
		}

		//GenerateHeightmap();

		auto baseName{GetParent()->GetName()};
		auto sideLength{2.0f * m_radius};
		auto squareSize{SquareRadiusRatio * m_radius};

		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, m_radius, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Top");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, -m_radius, 0.0f), Vector3(180.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Bottom");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, 0.0f, m_radius), Vector3(90.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Back");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, 0.0f, -m_radius), Vector3(270.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Front");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 270.0f)), 0, sideLength, squareSize, "Right");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(-m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 90.0f)), 0, sideLength, squareSize, "Left");
	}

	void Planet::Update()
	{
	}

	Colour Planet::GetColour(const Vector3f &cartesian)
	{
		//float u = ((std::atan2(cartesian.m_z / m_radius, cartesian.m_x / m_radius) / PI) + 1.0f) / 2.0f;
		//float v = 0.5f - (std::asin(cartesian.m_y / m_radius) / PI);

		return Colour::Clear; //  m_heightmap[uint32_t(u * m_heightmapSize) * (m_heightmapSize - 1) + uint32_t(v * m_heightmapSize)];
	}

	float Planet::GetRadius(const Vector3f &cartesian)
	{
		auto heightmap{GetColour(cartesian)};
		auto height{38.0f * ((2.0f * heightmap.m_r) - 1.0f)};
		return m_radius + height;
	}

	const Metadata &operator>>(const Metadata &metadata, Planet &planet)
	{
		return metadata;
	}

	Metadata &operator<<(Metadata &metadata, const Planet &planet)
	{
		return metadata;
	}
}
