#include "LodBehaviour.hpp"

#include "Scenes/Scenes.hpp"
#include "Maths/Maths.hpp"
#include "Meshes/Mesh.hpp"
#include "MeshTerrain.hpp"

namespace test
{
	LodBehaviour::LodBehaviour(const float &sideLength, const float &radius, const Transform &transform) :
		IBehaviour(),
		m_sideLength(sideLength),
		m_radius(radius),
		m_transform(Transform(transform)),
		m_modelLods(std::vector<std::shared_ptr<Model>>()),
		m_currentLod(5)
	{
		for (int i = 0; i < static_cast<int>(MeshTerrain::SQUARE_SIZES.size()); i++)
		{
			m_modelLods.emplace_back(nullptr);
		}
	}

	LodBehaviour::~LodBehaviour()
	{
	}

	void LodBehaviour::Update()
	{
		Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
		Vector3 chunkPosition = m_transform.GetPosition().ProjectCubeToSphere(m_radius) + GetGameObject()->GetTransform()->GetPosition();
		float distance = std::fabs(chunkPosition.Distance(cameraPosition));

		float lod = std::floor(0.75f * distance / m_sideLength);
		lod = Maths::Clamp(lod, 0.0f, static_cast<float>(MeshTerrain::SQUARE_SIZES.size() - 1));
		unsigned int lodi = static_cast<unsigned int>(lod);

		if (lodi != m_currentLod)
		{
			if (m_modelLods.at(lodi) == nullptr)
			{
				CreateLod(lodi, static_cast<float>(m_sideLength));
			}

			auto mesh = GetGameObject()->GetComponent<Mesh>();

			if (mesh != nullptr)
			{
				mesh->SetModel(m_modelLods.at(lodi));
			}

			m_currentLod = lodi;
		}
	}

	void LodBehaviour::CreateLod(const unsigned int &lod, const float &sideLength)
	{
		if (m_modelLods.at(lod) != nullptr)
		{
			return;
		}

#if FL_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif
		float squareSize = MeshTerrain::SQUARE_SIZES.at(lod);
		float textureScale = MeshTerrain::TEXTURE_SCALES.at(lod);
		int vertexCount = CalculateVertexCount(sideLength, squareSize);
		float lodFixScale = 1.0f; // (lod == 0) ? 1.0f : 1.02f + (0.028f * lod);
		m_modelLods.at(lod) = std::make_shared<MeshTerrain>(lodFixScale * sideLength, lodFixScale * squareSize, vertexCount, textureScale, m_radius, m_transform);
#if FL_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();

		if (debugEnd - debugStart > 22.0f)
		{
			printf("Terrain LOD %i built in %fms\n", lod, debugEnd - debugStart);
		}
#endif
	}

	int LodBehaviour::CalculateVertexCount(const float &sideLength, const float &squareSize)
	{
		return static_cast<int>((2.0f * sideLength) / static_cast<float>(squareSize)) + 1;
	}
}
