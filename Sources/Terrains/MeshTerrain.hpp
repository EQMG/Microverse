#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Maths.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Transform.hpp>

using namespace acid;

namespace test
{
	class MeshTerrain :
		public MeshSimple
	{
	private:
		float m_radius;
		Matrix4 m_worldMatrix;
	public:
		MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, const Transform &transform);

		~MeshTerrain();

		MeshSimpleVertex CalculateVertex(const float &x, const float &z) override;
	};
}