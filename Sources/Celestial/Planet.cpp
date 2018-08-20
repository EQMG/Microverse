#include "Planet.hpp"

#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelRectangle.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Models/Obj/ModelObj.hpp>
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"
#include "Star.hpp"

namespace test
{
	const float Planet::MEDIAN_RADIUS = 700.0f; // +- 50.0%
	const float Planet::MEDIAN_DENSITY = 5510.0f;
	const float Planet::MEDIAN_MASS = MEDIAN_MASS * (4.0f / 3.0f) * PI * std::pow(MEDIAN_RADIUS, 3.0f);
	const float Planet::SQUARE_RADIUS_RATIO = 0.2f; // 0.2 or 0.4;

	Planet::Planet(const int &seed, const float &radius, const float &density, const float &axialTilt) :
		ICelestial(),
		m_seed(seed),
		m_radius(radius),
		m_density(density),
		m_mass(m_density * (4.0f / 3.0f) * PI * std::pow(m_radius, 3.0f)),
		m_escapeVelocity(std::sqrt(2.0f * Star::G_CONSTANT * m_mass / m_radius)),
		m_axialTilt(axialTilt),
		m_axialTropics(axialTilt),
		m_axialPolar(90.0f - axialTilt),
		m_innerRings(1.34f * (m_radius / MEDIAN_RADIUS)),
		m_outterRings(2.44f * (m_radius / MEDIAN_RADIUS))
	{
		float surfaceGravity = Star::G_CONSTANT * m_mass / std::pow(m_radius, 2.0f);

		fprintf(stdout, "Planet: Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Surface Gravity(m/s^2)=%f, Escape Velocity(m/s)=%f\n", m_radius, m_density, m_mass, surfaceGravity, m_escapeVelocity);
	}

	Planet::~Planet()
	{
	}

	void Planet::Start()
	{
		if (m_seed == -1)
		{
			auto mesh = GetGameObject()->GetComponent<Mesh>();

			if (mesh != nullptr)
			{
				mesh->SetModel(ModelSphere::Resource(32, 32, m_radius));
			//	mesh->SetModel(ModelObj::Resource("Icosphere.obj"));
			//	GetGameObject()->GetTransform().SetScaling(Vector3(m_radius, m_radius, m_radius));
			}

			return;
		}

		std::string baseName = GetGameObject()->GetName();
		float sideLength = 2.0f * m_radius;
		float squareSize = SQUARE_RADIUS_RATIO * m_radius;

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

	void Planet::Load(LoadedValue *value)
	{
		// TODO
	}

	void Planet::Write(LoadedValue *destination)
	{
	}
}
