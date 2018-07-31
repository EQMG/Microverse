#include "LodBehaviour.hpp"

#include <algorithm>
#include <Scenes/Scenes.hpp>
#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include "MeshTerrain.hpp"
#include "MaterialTerrain.hpp"

namespace test
{
	static const unsigned int MAX_LOD = 3;

	LodBehaviour::LodBehaviour(const unsigned int &lod, const float &sideLength, const float &radius, const float &squareSize, const Transform &transform) :
		IBehaviour(),
		m_lod(lod),
		m_sideLength(sideLength),
		m_radius(radius),
		m_squareSize(squareSize),
		m_transform(Transform(transform)),
		m_children(std::array<GameObject *, 4>()),
		m_subdivided(false)
	{
	}

	LodBehaviour::~LodBehaviour()
	{
	}

	void LodBehaviour::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr && mesh->GetModel() == nullptr)
		{
#if ACID_VERBOSE
			float debugStart = Engine::Get()->GetTimeMs();
#endif
			int vertexCount = CalculateVertexCount(m_sideLength, m_squareSize);
			float textureScale = CalculateTextureScale(m_sideLength);
			float lodFixScale = 1.0f; // 1.0f + (0.016f * m_lod);
			mesh->SetModel(std::make_shared<MeshTerrain>(lodFixScale * m_sideLength, lodFixScale * m_squareSize, vertexCount, textureScale, m_radius, m_transform));
#if ACID_VERBOSE
			float debugEnd = Engine::Get()->GetTimeMs();

			if (debugEnd - debugStart > 22.0f)
			{
				printf("Terrain built in %fms\n", debugEnd - debugStart);
			}
#endif
		}
	}

	void LodBehaviour::Update()
	{
		auto meshRender = GetGameObject()->GetComponent<MeshRender>(true);

		if (meshRender != nullptr)
		{
			meshRender->SetEnabled(!m_subdivided);
		}

		unsigned int targetLod = GetCameraLod(this);

		auto materialTerrain = GetGameObject()->GetComponent<MaterialTerrain>();

		if (materialTerrain != nullptr)
		{
			if (targetLod == 0)
				materialTerrain->SetBaseColor(Colour::BLUE);
			else if (targetLod == 1)
				materialTerrain->SetBaseColor(Colour::GREEN);
			else if (targetLod == 2)
				materialTerrain->SetBaseColor(Colour::YELLOW);
			else if (targetLod == 3)
				materialTerrain->SetBaseColor(Colour::RED);
		}

		if (targetLod > m_lod && m_lod < MAX_LOD && !m_subdivided)
		{
			Subdivide();
		}
		else if (targetLod == m_lod && m_subdivided)
		{
			Merge();
		}
	}

	unsigned int LodBehaviour::GetCameraLod(LodBehaviour *behaviour)
	{
		Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
		Vector3 chunkPosition = behaviour->m_transform.GetPosition().ProjectCubeToSphere(behaviour->m_radius) + behaviour->GetGameObject()->GetTransform().GetPosition();
		float distance = std::fabs(chunkPosition.Distance(cameraPosition));
		float lod = std::floor((-4.0f / 2800.0f) * distance + 4.0f);
		return static_cast<unsigned int>(std::max(lod, 0.0f));
	}

	void LodBehaviour::Subdivide()
	{
		int subLod = m_lod + 1;
		float subSideLength = m_sideLength / 2.0f;

		for (int i = 0; i < 4; i++)
		{
			Vector3 subOffset = Vector3(i < 2 ? 1.0f : -1.0f, 0.0f, i % 2 ? 1.0f : -1.0f);
			subOffset = subOffset.Rotate(m_transform.GetRotation());
			subOffset *= 0.5f * subSideLength;
			Transform subTransform = Transform(m_transform.GetPosition() + subOffset, m_transform.GetRotation(), m_transform.GetScaling());

			GameObject *subChunk = new GameObject(Transform());
			subChunk->SetName(GetGameObject()->GetName() + "_" + std::to_string(i));
			subChunk->SetParent(GetGameObject());
			subChunk->AddComponent<Mesh>();
			subChunk->AddComponent<LodBehaviour>(subLod, subSideLength, m_radius, m_squareSize / 2.0f, subTransform);
			subChunk->AddComponent<MaterialTerrain>();
			subChunk->AddComponent<MeshRender>();
			//subChunk->AddComponent<ShadowRender>();

			m_children[i] = subChunk;
		}

		m_subdivided = true;
	}

	void LodBehaviour::Merge()
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_children[i] == nullptr)
			{
				continue;
			}

			auto lodBehaviour = m_children[i]->GetComponent<LodBehaviour>();

			if (lodBehaviour != nullptr)
			{
				lodBehaviour->Merge();
			}

			delete m_children[i];
			m_children[i] = nullptr;
		}

		m_subdivided = false;
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
