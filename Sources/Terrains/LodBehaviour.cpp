#include "LodBehaviour.hpp"

#include "Scenes/Scenes.hpp"
#include "Maths/Maths.hpp"
#include "Meshes/Mesh.hpp"
#include "MeshTerrain.hpp"

namespace test
{
	LodBehaviour::LodBehaviour(const float &sideLength, const float &radius, const float &squareSize, const Transform &transform) :
		IBehaviour(),
		m_sideLength(sideLength),
		m_radius(radius),
		m_squareSize(squareSize),
		m_transform(Transform(transform))
	{
	}

	LodBehaviour::~LodBehaviour()
	{
	}

	void LodBehaviour::Update()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh != nullptr && mesh->GetModel() == nullptr)
		{
#if FL_VERBOSE
			float debugStart = Engine::Get()->GetTimeMs();
#endif
			int vertexCount = CalculateVertexCount(m_sideLength, m_squareSize);
			float textureScale = CalculateTextureScale(m_sideLength);
			float lodFixScale = 1.0f; // (lod == 0) ? 1.0f : 1.02f + (0.028f * lod);
			mesh->SetModel(std::make_shared<MeshTerrain>(lodFixScale * m_sideLength, lodFixScale * m_squareSize, vertexCount, textureScale, m_radius, m_transform));
#if FL_VERBOSE
			float debugEnd = Engine::Get()->GetTimeMs();

			if (debugEnd - debugStart > 22.0f)
			{
				printf("Terrain built in %fms\n", debugEnd - debugStart);
			}
#endif
			mesh->SetEnabled(false);
		}
	}

	int LodBehaviour::CalculateVertexCount(const float &sideLength, const float &squareSize)
	{
		return static_cast<int>((2.0f * sideLength) / static_cast<float>(squareSize)) + 1;
	}

	float LodBehaviour::CalculateTextureScale(const float &squareSize)
	{
		return 35.362f * std::pow(squareSize, -0.915f);
	}
}
