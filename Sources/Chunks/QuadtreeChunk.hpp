#pragma once

#include <array>
#include <memory>
#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>
#include "Celestial/Planet.hpp"
#include "MeshChunk.hpp"

using namespace acid;

namespace test
{
	class QuadtreeChunk :
		public IComponent
	{
	private:
		Planet *m_parent;

		uint32_t m_lod;
		float m_sideLength;
		float m_squareSize;
		Transform m_transform;

		std::vector<QuadtreeChunk *> m_children;
		bool m_subdivided;
		bool m_visible;
		float m_lastChanged;
	public:
		static const uint32_t HIGHEST_LOD;
		static const float DELAY_RENDER;
		static const float DELAY_PURGE;
		static const std::vector<Vector3> OFFSETS;

		QuadtreeChunk(Planet *parent = nullptr, const uint32_t &lod = 0, const float &sideLength = 200.0f, const float &squareSize = 4.0f, const Transform &transform = Transform());

		~QuadtreeChunk();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		static GameObject *CreateChunk(Planet *parent, const Transform &transform, const uint32_t &lod = 0, const float &sideLength = 100.0f, const float &squareSize = 100.0f, const std::string &namePostfix = "");

		uint32_t CalculateLod();

		void SetVisible(const bool &visible, const float &timeout);

		void DeleteChildren();

		void Subdivide();

		void Merge();
	private:
		static int CalculateVertexCount(const float &sideLength, const float &squareSize);

		static float CalculateTextureScale(const float &squareSize);
	};
}
