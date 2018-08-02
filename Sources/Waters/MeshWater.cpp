#include "MeshWater.hpp"

namespace test
{
	const float MeshWater::SIDE_LENGTH = 1200.0f;
	const float MeshWater::SQUARE_SIZE = 100.0f;
	const int MeshWater::VERTEX_COUNT = static_cast<int>((2.0f * SIDE_LENGTH) / SQUARE_SIZE) + 1;
	const float MeshWater::TEXTURE_SCALE = 1.0f;
	const Colour MeshWater::WATER_COLOUR = Colour("#0077be");

	MeshWater::MeshWater() :
		MeshSimple(SIDE_LENGTH, SQUARE_SIZE, VERTEX_COUNT, TEXTURE_SCALE)
	{
		MeshSimple::GenerateMesh();
	}

	MeshWater::~MeshWater()
	{
	}

	MeshSimpleVertex MeshWater::CalculateVertex(const float &x, const float &z)
	{
		MeshSimpleVertex result = {};
		result.position = Vector3(x, 0.0f, z);
		result.normal = Vector3(0.0f, 1.0f, 0.0f);
		result.colour = WATER_COLOUR;
		return result;
	}
}
