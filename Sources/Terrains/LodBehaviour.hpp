#pragma once

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
		float m_sideLength;
		float m_radius;
		float m_squareSize;
		Transform m_transform;
	public:
		LodBehaviour(const float &sideLength = 200.0f, const float &radius = 0.0f, const float &squareSize = 4.0f, const Transform &transform = Transform());

		~LodBehaviour();

		void Update() override;
	private:
		static int CalculateVertexCount(const float &sideLength, const float &squareSize);

		static float CalculateTextureScale(const float &squareSize);
	};
}
