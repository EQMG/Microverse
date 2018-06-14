#pragma once

#include <Objects/Behaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>

using namespace fl;

namespace test
{
	class LodBehaviour :
		public Behaviour
	{
	private:
		float m_radius;
		Transform m_transform;
		std::vector<std::shared_ptr<Model>> m_modelLods;
		unsigned int m_currentLod;
	public:
		LodBehaviour(const float &radius = 0.0f, const Transform &transform = Transform());

		~LodBehaviour();

		void Update() override;
	private:
		void CreateLod(const unsigned int &lod);

		static int CalculateVertexCount(const int &terrainLength, const float &squareSize);
	};
}
