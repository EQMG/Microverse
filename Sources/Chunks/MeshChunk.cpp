#include "MeshChunk.hpp"

#include "World/World.hpp"

namespace micro
{
	const std::array<Colour, 4> COLOUR_BIOMES =
	{
		Colour("#6e3529"),
		Colour("#934838"),
		Colour("#9e402c"),
		Colour("#656565")
	};
	const float COLOUR_SPREAD = 0.76f;
	const float COLOUR_HALF_SPREAD = COLOUR_SPREAD / 2.0f;
	const float COLOUR_AMPLITUDE = 28.0f;
	const float COLOUR_PART = 1.0f / (COLOUR_BIOMES.size() - 1);

	MeshChunk::MeshChunk(Planet *planet, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale, const Transform &transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_parent(planet),
		m_worldMatrix(transform.GetWorldMatrix()),
		m_edgeConditions(EDGE_EAST | EDGE_SOUTH)
	{
		MeshSimple::GenerateMesh();
	}

	MeshChunk::~MeshChunk()
	{
	}

	VertexModel *MeshChunk::GetVertex(const uint32_t &col, const uint32_t &row)
	{
		float x = ((row * m_squareSize) - m_sideLength) / 2.0f;
		float z = ((col * m_squareSize) - m_sideLength) / 2.0f;

		Vector3 cartesian = Vector3(m_worldMatrix.Multiply(Vector4(x, 0.0f, z, 1.0f)));
		cartesian = cartesian.ProjectCubeToSphere(m_parent->GetRadius());

		Vector3 polar = cartesian.CartesianToPolar();
		polar.m_x = m_parent->GetRadius(cartesian);
		Vector3 position = polar.PolarToCartesian();

		Vector2 uv = Vector2(
			((std::atan2(cartesian.m_z / m_parent->GetRadius(), cartesian.m_x / m_parent->GetRadius()) / PI) + 1.0f) / 2.0f,
			0.5f - (std::asin(cartesian.m_y / m_parent->GetRadius()) / PI)
		); // Vector2::ZERO
		Vector3 normal = GetNormal(x, z); // Vector3::ZERO
		Colour colour = m_parent->GetColour(cartesian); // Colour::WHITE
		return new VertexModel(position, uv, normal, colour);
	}

	Vector3 MeshChunk::GetPosition(const float &x, const float &z)
	{
		Vector3 cartesian = Vector3(m_worldMatrix.Multiply(Vector4(x, 0.0f, z, 1.0f)));

		if (m_parent->GetRadius() == 0.0f)
		{
			return cartesian;
		}

		cartesian = cartesian.ProjectCubeToSphere(m_parent->GetRadius());
		Vector3 polar = cartesian.CartesianToPolar();
		polar.m_x = m_parent->GetRadius(cartesian);
		return polar.PolarToCartesian();
	}

	Vector3 MeshChunk::GetNormal(const float &x, const float &z)
	{
		Vector3 positionL = GetPosition(x - 1.0f, z);
		Vector3 positionR = GetPosition(x + 1.0f, z);
		Vector3 positionD = GetPosition(x, z - 1.0f);
	//	Vector3 positionU = GetPosition(x, z + 1.0f);

		Vector3 normal = (positionL - positionR).Cross(positionR - positionD);
		return normal.Normalize();
	}
}
