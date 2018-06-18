#pragma once

#include <Objects/IBehaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>

using namespace fl;

namespace test
{
	class LodBehaviour :
		public IBehaviour
	{
	private:
		float m_sideLength;
		float m_radius;
		Transform m_transform;
		std::vector<std::shared_ptr<Model>> m_modelLods;
		unsigned int m_currentLod;
	public:
		LodBehaviour(const float &sideLength = 200.0f, const float &radius = 0.0f, const Transform &transform = Transform());

		~LodBehaviour();

		void Update() override;
	private:
		void CreateLod(const unsigned int &lod, const float &sideLength);

		static int CalculateVertexCount(const float &sideLength, const float &squareSize);
	};
}
