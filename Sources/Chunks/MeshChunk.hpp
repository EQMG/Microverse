#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Maths.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Transform.hpp>
#include "Celestial/Planet.hpp"

using namespace acid;

namespace micro
{
	enum EdgeFlagBits
	{
		None = 0,
		North = 1,
		East = 2,
		South = 4,
		West = 8
	};
	typedef uint32_t EdgeFlags;

	class MeshChunk :
		public MeshSimple
	{
	public:
		MeshChunk(Planet *parent, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale, const Transform &transform);

		VertexDefault GetVertex(const uint32_t &col, const uint32_t &row) override;
	private:
		Vector3f GetPosition(const float &x, const float &z);

		Vector3f GetNormal(const float &x, const float &z);

		Vector3f ProjectCubeToSphere(const Vector3f &source, const float &radius);

		Planet *m_parent;

		Matrix4 m_worldMatrix;
		EdgeFlags m_edgeConditions;
	};
}