#include "MeshTerrain.hpp"

#include <World/World.hpp>

namespace test
{
	const std::array<Colour, 4> COLOUR_BIOMES = {
		Colour("#6e3529"), Colour("#934838"), Colour("#9e402c"), Colour("#656565")
	};
	const float COLOUR_SPREAD = 0.76f;
	const float COLOUR_HALF_SPREAD = COLOUR_SPREAD / 2.0f;
	const float COLOUR_AMPLITUDE = 28.0f;
	const float COLOUR_PART = 1.0f / (COLOUR_BIOMES.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, const Transform &transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_radius(radius),
		m_worldMatrix(transform.GetWorldMatrix())
	{
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
	}

	MeshSimpleVertex MeshTerrain::CalculateVertex(const float &x, const float &z)
	{
		auto calculatePosition = [&](float col, float row){
			Vector4 cartesian = m_worldMatrix.Multiply(Vector4(col, 0.0f, row, 1.0f));
			Vector3 polar = Vector3(cartesian).ProjectCubeToSphere(m_radius).CartesianToPolar();
			polar.m_x = World::Get()->GetTerrainRadius(m_radius, polar.m_y, polar.m_z);
			return polar.PolarToCartesian();
		};

		MeshSimpleVertex result = {};

		if (m_radius != 0.0f)
		{
			Vector4 cartesian = m_worldMatrix.Multiply(Vector4(x, 0.0f, z, 1.0f));
			Vector3 polar = Vector3(cartesian).ProjectCubeToSphere(m_radius).CartesianToPolar();
			polar.m_x = World::Get()->GetTerrainRadius(m_radius, polar.m_y, polar.m_z);
			result.position = polar.PolarToCartesian();

			float value = (polar.m_x - m_radius + COLOUR_AMPLITUDE) / (COLOUR_AMPLITUDE * 2.0f);
			value = Maths::Clamp((value - COLOUR_HALF_SPREAD) * (1.0f / COLOUR_SPREAD), 0.0f, 0.9999f);
			int firstBiome = static_cast<int>(std::floor(value / COLOUR_PART));
			float blend = (value - (firstBiome * COLOUR_PART)) / COLOUR_PART;
			result.colour = COLOUR_BIOMES.at(firstBiome).Interpolate(COLOUR_BIOMES.at(firstBiome + 1), blend);
		}
		else
		{
			result.position = m_worldMatrix.Multiply(Vector3(x, 0.0f, z));
			result.colour = Colour::WHITE;
		}

		Vector3 positionL = calculatePosition(x - 1.0f, z);
		Vector3 positionR = calculatePosition(x + 1.0f, z);
		Vector3 positionD = calculatePosition(x, z - 1.0f);
	//	Vector3 positionU = calculatePosition(x, z + 1.0f);

		Vector3 normal = (positionL - positionR).Cross(positionR - positionD);
		result.normal = normal.Normalize();
	//	result.normal = Vector3::ZERO;

		return result;
	}
}
