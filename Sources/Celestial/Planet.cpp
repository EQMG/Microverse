#include "Planet.hpp"

#include <Maths/Maths.hpp>
#include <Objects/GameObject.hpp>
#include <Physics/Rigidbody.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Noise/Noise.hpp>
#include <Helpers/FileSystem.hpp>
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"

namespace test
{
	const float Planet::MEDIAN_RADIUS = 600.0f; // +- 50.0%
	const float Planet::SQUARE_RADIUS_RATIO = 0.2f; // Or 0.4f;

	Planet::Planet(Star *star, const float &radius, const float &density) :
		ICelestial(),
		m_star(star),
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

	std::shared_ptr<Texture> Planet::GenerateMap()
	{
		Noise noiseTerrain = Noise(69124);
		noiseTerrain.SetNoiseType(NoiseType::TYPE_PERLINFRACTAL);
		noiseTerrain.SetFrequency(0.00625f);
		noiseTerrain.SetInterp(NoiseInterp::INTERP_QUINTIC);
		noiseTerrain.SetFractalType(NoiseFractal::FRACTAL_FBM);
		noiseTerrain.SetFractalOctaves(4);
		noiseTerrain.SetFractalLacunarity(1.8f);
		noiseTerrain.SetFractalGain(0.6f);

		Noise noiseBiome = Noise(56744);
		noiseBiome.SetNoiseType(NoiseType::TYPE_SIMPLEXFRACTAL);
		noiseBiome.SetFrequency(0.001f);
		noiseBiome.SetInterp(NoiseInterp::INTERP_QUINTIC);
		noiseBiome.SetFractalType(NoiseFractal::FRACTAL_FBM);
		noiseBiome.SetFractalOctaves(2);
		noiseBiome.SetFractalLacunarity(2.0f);
		noiseBiome.SetFractalGain(0.5f);

		std::string filename = FileSystem::GetWorkingDirectory() + "/" + GetGameObject()->GetName() + ".png";
		int width = 2500;
		int height = 2500;
		unsigned int *data = (unsigned int *) malloc(width * height * 4);

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				float random = (noiseTerrain.GetNoise(x, y) + 1.0f) / 2.0f;
				float r = random * ((noiseBiome.GetNoise(x, y) + 1.0f) / 2.0f);
				float g = random * ((noiseBiome.GetNoise(x * 10.0f, y * 10.0f) + 1.0f) / 2.0f);
				float b = random * ((noiseBiome.GetNoise(x * 33.0f, y * 33.0f) + 1.0f) / 2.0f);

				unsigned char buffer[4];
				buffer[0] = std::floor(255 * Maths::Clamp(r, 0.0f, 1.0f));
				buffer[1] = std::floor(255 * Maths::Clamp(g, 0.0f, 1.0f));
				buffer[2] = std::floor(255 * Maths::Clamp(b, 0.0f, 1.0f));
				buffer[3] = 255;
				memcpy((char*)&data[x * width + y], buffer, 4);
			}
		}

		FileSystem::DeleteFile(filename);
		Texture::WritePixels(filename, data, width, height, 4);
		free(data);

		return Texture::Resource(filename);
	}
}
