#pragma once

#include <array>
#include <Objects/IBehaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>

using namespace acid;

namespace test
{
	class LodBehaviour :
		public IBehaviour
	{
	private:
		unsigned int m_lod;
		float m_sideLength;
		float m_radius;
		float m_squareSize;
		Transform m_transform;

		std::array<GameObject *, 4> m_children;
		bool m_subdivided;
	public:
		static const unsigned int HIGHEST_LOD;

		LodBehaviour(const unsigned int &lod = 0, const float &sideLength = 200.0f, const float &radius = 0.0f, const float &squareSize = 4.0f, const Transform &transform = Transform());

		~LodBehaviour();

		void Start() override;

		void Update() override;

		static float GetSideRadiusRatio(const float &radius);

		static unsigned int GetCameraLod(LodBehaviour *behaviour);

		void Subdivide();

		void Merge();
	private:
		static int CalculateVertexCount(const float &sideLength, const float &squareSize);

		static float CalculateTextureScale(const float &squareSize);
	};
}
