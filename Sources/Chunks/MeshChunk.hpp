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
		EDGE_NONE = 0,
		EDGE_NORTH = 1,
		EDGE_EAST = 2,
		EDGE_SOUTH = 4,
		EDGE_WEST = 8
	};
	typedef uint32_t EdgeFlags;

	class MeshChunk :
		public MeshSimple
	{
	private:
		Planet *m_parent;

		Matrix4 m_worldMatrix;
		EdgeFlags m_edgeConditions;
	public:
		MeshChunk(Planet *parent, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale, const Transform &transform);

		~MeshChunk();

		VertexModel *GetVertex(const uint32_t &col, const uint32_t &row) override;
	private:
		Vector3 GetPosition(const float &x, const float &z);

		Vector3 GetNormal(const float &x, const float &z);
	};
}