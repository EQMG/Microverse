#include "QuadtreeChunk.hpp"

#include <Scenes/Scenes.hpp>
#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include "MaterialChunk.hpp"

namespace micro
{
	const uint32_t QuadtreeChunk::HIGHEST_LOD{3};
	const Time QuadtreeChunk::DELAY_RENDER{0.3s};
	const Time QuadtreeChunk::DELAY_PURGE{6.0s};
	const std::vector<Vector3f> QuadtreeChunk::OFFSETS{
		{1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, -1.0f},
		{-1.0f, 0.0f, -1.0f},
		{-1.0f, 0.0f, 1.0f}
	};

	QuadtreeChunk::QuadtreeChunk(Planet *parent, const uint32_t &lod, const float &sideLength, const float &squareSize, const Transform &transform) :
		m_parent{parent},
		m_lod{lod},
		m_sideLength{sideLength},
		m_squareSize{squareSize},
		m_transform{transform}
	{
	}

	void QuadtreeChunk::Start()
	{
		m_lastChanged = Time::Now();

		auto mesh = GetParent()->GetComponent<Mesh>(true);

		if (mesh == nullptr)
		{
			Log::Error("Lod Behaviour must be attached to a object with a mesh!");
			return;
		}

#if defined(ACID_VERBOSE)
		auto debugStart{Time::Now()};
#endif
		auto vertexCount{CalculateVertexCount(m_sideLength, m_squareSize)};
		auto textureScale{CalculateTextureScale(m_sideLength)};
		mesh->SetModel(std::make_shared<MeshChunk>(m_parent, m_sideLength, m_squareSize, vertexCount, textureScale, m_transform));

#if defined(ACID_VERBOSE)
		auto debugEnd{Time::Now()};

		if ((debugEnd - debugStart).AsMilliseconds() > 10.0f)
		{
			Log::Out("Terrain built in %.3fms\n", (debugEnd - debugStart).AsMilliseconds<float>());
		}
#endif
	}

	void QuadtreeChunk::Update()
	{
		GetParent()->SetLocalTransform(m_parent->GetParent()->GetTransform());

		if (!m_subdivided && !m_children.empty() && Engine::GetTime() - m_lastChanged > DELAY_PURGE)
		{
			m_children.clear();
			return;
		}

		auto targetLod = CalculateLod();

		if (targetLod > m_lod && !m_subdivided)
		{
			Subdivide();
		}
		else if (targetLod == m_lod && m_subdivided)
		{
			Merge();
		}
	}

	Entity *QuadtreeChunk::CreateChunk(Planet *parent, const Transform &transform, const uint32_t &lod, const float &sideLength, const float &squareSize, const std::string &namePostfix)
	{
		GameObject *terrainChunk = new GameObject(Transform());
		terrainChunk->SetName(parent->GetGameObject()->GetName() + "_" + namePostfix);
		terrainChunk->SetParent(parent->GetGameObject());
		terrainChunk->AddComponent<Mesh>();
		terrainChunk->AddComponent<QuadtreeChunk>(parent, lod, sideLength, squareSize, transform);
	//	terrainChunk->AddComponent<ColliderConvexHull>(transform.GetPosition());
	//	terrainChunk->AddComponent<Rigidbody>(0.0f);
		terrainChunk->AddComponent<MaterialChunk>();
		terrainChunk->AddComponent<MeshRender>();
	//	terrainChunk->AddComponent<ShadowRender>();
		return terrainChunk;
	}

	uint32_t QuadtreeChunk::CalculateLod()
	{
		Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
		Matrix4 worldMatrix = GetGameObject()->GetTransform().GetWorldMatrix();
		Vector3 chunkPosition = m_transform.GetPosition().ProjectCubeToSphere(m_parent->GetRadius());
		chunkPosition = Vector3(worldMatrix.Multiply(chunkPosition));
		float distance = std::fabs(chunkPosition.Distance(cameraPosition));
		float lod = std::floor((-1.618f / m_parent->GetRadius()) * distance + (HIGHEST_LOD + 1));
		return static_cast<uint32_t>(std::clamp(lod, 0.0f, static_cast<float>(HIGHEST_LOD)));
	}

	void QuadtreeChunk::SetVisible(const bool &visible, const Time &timeout)
	{
		m_visible = visible;

		if (timeout == Time::ZERO)
		{
			auto meshRender = GetGameObject()->GetComponent<MeshRender>(true);

			if (meshRender != nullptr)
			{
				meshRender->SetEnabled(m_visible);
			}

			return;
		}

		Events::Get()->AddEvent<EventTime>(timeout, [&](){
			if (m_visible != visible)
			{
				return;
			}

			auto meshRender = GetGameObject()->GetComponent<MeshRender>(true);

			if (meshRender != nullptr)
			{
				meshRender->SetEnabled(m_visible);
			}
		}, false);
	}

	void QuadtreeChunk::Subdivide()
	{
		m_subdivided = true;
		m_lastChanged = Engine::GetTime();
		SetVisible(false, DELAY_RENDER);

		if (!m_children.empty())
		{
			for (auto &child : m_children)
			{
				child->SetVisible(true, Time::ZERO);
				child->GetGameObject()->SetStructure(Scenes::Get()->GetStructure());
			}

			return;
		}

		for (auto &offset : OFFSETS)
		{
			Vector3 childOffset = offset.Rotate(m_transform.GetRotation());
			childOffset *= 0.25f * m_sideLength;
			Transform childTransform = Transform(m_transform.GetPosition() + childOffset, m_transform.GetRotation(), m_transform.GetScaling());

			GameObject *child = CreateChunk(m_parent, childTransform, m_lod + 1, m_sideLength / 2.0f, m_squareSize / 2.0f, offset.ToString());
			m_children.emplace_back(child->GetComponent<QuadtreeChunk>());
		}
	}

	void QuadtreeChunk::Merge()
	{
		m_subdivided = false;
		m_lastChanged = Engine::GetTime();
		SetVisible(true, Time::ZERO);

		for (auto &child : m_children)
		{
			child->Merge();
			child->SetVisible(false, Time::ZERO);
			child->GetGameObject()->SetRemoved(true);
		}
	}

	uint32_t QuadtreeChunk::CalculateVertexCount(const float &sideLength, const float &squareSize)
	{
		return static_cast<uint32_t>((2.0f * sideLength) / static_cast<float>(squareSize)) + 1;
	}

	float QuadtreeChunk::CalculateTextureScale(const float &squareSize)
	{
		return 35.362f * std::pow(squareSize, -0.915f);
	}
}
