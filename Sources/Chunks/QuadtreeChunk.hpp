#pragma once

#include <array>
#include <memory>
#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>
#include <Models/Model.hpp>
#include <Maths/Time.hpp>
#include "Celestial/Planet.hpp"
#include "MeshChunk.hpp"

using namespace acid;

namespace micro
{
	class QuadtreeChunk :
		public Component
	{
	public:
		static const uint32_t HIGHEST_LOD;
		static const Time DELAY_RENDER;
		static const Time DELAY_PURGE;
		static const std::vector<Vector3f> OFFSETS;

		QuadtreeChunk(Planet *parent = nullptr, const uint32_t &lod = 0, const float &sideLength = 200.0f, const float &squareSize = 4.0f, const Transform &transform = Transform());

		void Start() override;

		void Update() override;

		static Entity *CreateChunk(Planet *parent, const Transform &transform, const uint32_t &lod = 0, const float &sideLength = 100.0f, const float &squareSize = 100.0f, const std::string &namePostfix = "");

		uint32_t CalculateLod();

		void SetVisible(const bool &visible, const Time &timeout);

		void Subdivide();

		void Merge();
	private:
		static uint32_t CalculateVertexCount(const float &sideLength, const float &squareSize);

		static float CalculateTextureScale(const float &squareSize);

		Planet *m_parent;

		uint32_t m_lod;
		float m_sideLength;
		float m_squareSize;
		Transform m_transform;

		std::vector<QuadtreeChunk *> m_children;
		bool m_subdivided{};
		bool m_visible{};
		Time m_lastChanged;
	};
}
