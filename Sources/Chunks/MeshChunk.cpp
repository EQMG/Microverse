#include "MeshChunk.hpp"

#include "World/World.hpp"

namespace test
{
	const std::array<Colour, 4> COLOUR_BIOMES = {
		Colour("#6e3529"), Colour("#934838"), Colour("#9e402c"), Colour("#656565")
	};
	const float COLOUR_SPREAD = 0.76f;
	const float COLOUR_HALF_SPREAD = COLOUR_SPREAD / 2.0f;
	const float COLOUR_AMPLITUDE = 28.0f;
	const float COLOUR_PART = 1.0f / (COLOUR_BIOMES.size() - 1);

	MeshChunk::MeshChunk(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, const Transform &transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_radius(radius),
		m_worldMatrix(transform.GetWorldMatrix()),
		m_edgeConditions(EDGE_EAST | EDGE_SOUTH)
	{
		MeshSimple::GenerateMesh();
	}

	MeshChunk::~MeshChunk()
	{
	}

	Vector3 MeshChunk::GetPosition(const float &x, const float &z)
	{
		Vector3 cartesian = Vector3(m_worldMatrix.Multiply(Vector4(x, 0.0f, z, 1.0f)));

		if (m_radius == 0.0f)
		{
			return cartesian;
		}

		Vector3 polar = cartesian.ProjectCubeToSphere(m_radius).CartesianToPolar();
		polar.m_x = World::Get()->GetTerrainRadius(m_radius, polar.m_y, polar.m_z);
		return polar.PolarToCartesian();
	}

	Vector3 MeshChunk::GetNormal(const float &x, const float &z, const Vector3 &position)
	{
		Vector3 positionL = GetPosition(x - 1.0f, z);
		Vector3 positionR = GetPosition(x + 1.0f, z);
		Vector3 positionD = GetPosition(x, z - 1.0f);
	//	Vector3 positionU = calculatePosition(x, z + 1.0f);

		Vector3 normal = (positionL - positionR).Cross(positionR - positionD);
		return normal.Normalize();
	//	return Vector3::ZERO;
	}

	Vector3 MeshChunk::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		Vector3 polar = position.CartesianToPolar();
		float value = (polar.m_x - m_radius + COLOUR_AMPLITUDE) / (COLOUR_AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - COLOUR_HALF_SPREAD) * (1.0f / COLOUR_SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / COLOUR_PART));
		float blend = (value - (firstBiome * COLOUR_PART)) / COLOUR_PART;
		Colour colour = COLOUR_BIOMES.at(firstBiome).Interpolate(COLOUR_BIOMES.at(firstBiome + 1), blend);
		return colour;
	//	return Colour::WHITE;
	}
}
