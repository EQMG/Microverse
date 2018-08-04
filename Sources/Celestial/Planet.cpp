#include "Planet.hpp"

#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"

namespace test
{
	const float Planet::MEDIAN_RADIUS = 600.0f; // +- 50.0%
	const float Planet::SQUARE_RADIUS_RATIO = 0.2f; // Or 0.4f;

	Planet::Planet(Star *star, const int &seed, const float &radius, const float &density) :
		ICelestial(),
		m_star(star),
		m_seed(seed),
		m_radius(radius),
		m_density(density),
		m_mass(m_density * (4.0f / 3.0f) * PI * std::pow(m_radius, 3.0f)),
		m_surfaceGravity(Star::G_CONSTANT * m_mass / std::pow(m_radius, 2.0f)),
		m_escapeVelocity(std::sqrt(2.0f * Star::G_CONSTANT * m_mass / m_radius))
	{
		fprintf(stdout, "Planet: Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Surface Gravity(m/s^2)=%f, Escape Velocity(m/s)=%f\n", m_radius, m_density, m_mass, m_surfaceGravity, m_escapeVelocity);
	}

	Planet::~Planet()
	{
	}

	void Planet::Start()
	{
		if (m_seed == -1)
		{
			return;
		}

		std::string baseName = GetGameObject()->GetName();
		float sideLength = 2.0f * m_radius;
		float squareSize = SQUARE_RADIUS_RATIO * m_radius;

		CreateChunk(Transform(Vector3(0.0f, m_radius, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Top");
		CreateChunk(Transform(Vector3(0.0f, -m_radius, 0.0f), Vector3(180.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Bottom");
		CreateChunk(Transform(Vector3(0.0f, 0.0f, m_radius), Vector3(90.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Back");
		CreateChunk(Transform(Vector3(0.0f, 0.0f, -m_radius), Vector3(270.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Front");
		CreateChunk(Transform(Vector3(m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 270.0f)), 0, sideLength, squareSize, "Right");
		CreateChunk(Transform(Vector3(-m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 90.0f)), 0, sideLength, squareSize, "Left");
	}

	void Planet::Update()
	{
	}

	void Planet::Load(LoadedValue *value)
	{
	}

	void Planet::Write(LoadedValue *destination)
	{
	}

	GameObject *Planet::CreateChunk(const Transform &transform, const uint32_t &lod, const float &sideLength, const float &squareSize, const std::string &namePostfix)
	{
		GameObject *terrainChunk = new GameObject(Transform());
		terrainChunk->SetName(GetGameObject()->GetName() + "_" + namePostfix);
		terrainChunk->SetParent(GetGameObject());
		terrainChunk->AddComponent<Mesh>();
		terrainChunk->AddComponent<QuadtreeChunk>(this, lod, sideLength, squareSize, transform);
	//	terrainChunk->AddComponent<ColliderConvexHull>(transform.GetPosition());
	//	terrainChunk->AddComponent<Rigidbody>(0.0f);
		terrainChunk->AddComponent<MaterialChunk>();
		terrainChunk->AddComponent<MeshRender>();
	//	terrainChunk->AddComponent<ShadowRender>();
		return terrainChunk;
	}
}
