#include "Planet.hpp"

#include <Maths/Maths.hpp>
#include <Objects/GameObject.hpp>
#include <Physics/Rigidbody.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Noise/Noise.hpp>
#include <Helpers/FileSystem.hpp>
#include "QuadtreeChunk.hpp"
#include "MaterialChunk.hpp"

namespace test
{
	const float Planet::SQUARE_RADIUS_RATIO = 0.2f; // Or 0.4f;
	const float Planet::G_CONSTANT = 6.673e-11f;

	Planet::Planet(const float &radius, const float &density) :
		IComponent(),
		m_radius(radius),
		m_density(density),
		m_mass(m_density * (4.0f / 3.0f) * PI * std::pow(m_radius, 3.0f)),
		m_surfaceGravity(G_CONSTANT * m_mass / std::pow(m_radius, 2.0f))
	{
		fprintf(stdout, "Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Surface Gravity(m/s^2)=%f, Surface Orbit(m/s)=%f\n", m_radius, m_density, m_mass, m_surfaceGravity, std::sqrt(G_CONSTANT * m_mass / m_radius));
	}

	Planet::~Planet()
	{
	}

	void Planet::Start()
	{
		std::string baseName = GetGameObject()->GetName();
		float sideLength = 2.0f * m_radius;
		float squareSize = SQUARE_RADIUS_RATIO * m_radius;

		GameObject *chunkTop = CreateChunk(Transform(Vector3(0.0f, m_radius, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), 0, sideLength, squareSize);
		chunkTop->SetName(baseName + "_Top");

		GameObject *chunkBottom = CreateChunk(Transform(Vector3(0.0f, -m_radius, 0.0f), Vector3(180.0f, 0.0f, 0.0f)), 0, sideLength, squareSize);
		chunkBottom->SetName(baseName + "_Bottom");

		GameObject *chunkBack = CreateChunk(Transform(Vector3(0.0f, 0.0f, m_radius), Vector3(90.0f, 0.0f, 0.0f)), 0, sideLength, squareSize);
		chunkBack->SetName(baseName + "_Back");

		GameObject *chunkFront = CreateChunk(Transform(Vector3(0.0f, 0.0f, -m_radius), Vector3(270.0f, 0.0f, 0.0f)), 0, sideLength, squareSize);
		chunkFront->SetName(baseName + "_Front");

		GameObject *chunkRight = CreateChunk(Transform(Vector3(m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 270.0f)), 0, sideLength, squareSize);
		chunkRight->SetName(baseName + "_Right");

		GameObject *chunkLeft = CreateChunk(Transform(Vector3(-m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 90.0f)), 0, sideLength, squareSize);
		chunkLeft->SetName(baseName + "_Left");
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
