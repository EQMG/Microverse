#include "MeshTerrain.hpp"

#include <Worlds/Worlds.hpp>

namespace test
{
	const std::array<Colour, 20> BRIGHT_COLOURS = {
		Colour("#0048BA"), Colour("#B0BF1A"), Colour("#7CB9E8"), Colour("#C9FFE5"), Colour("#B284BE"),
		Colour("#5D8AA8"), Colour("#AF002A"), Colour("#C46210"), Colour("#E52B50"), Colour("#F19CBB"),
		Colour("#9F2B68"), Colour("#3B7A57"), Colour("#FFBF00"), Colour("#9966CC"), Colour("#008000"),
		Colour("#8F9779"), Colour("#FC419A"), Colour("#F28E1C"), Colour("#064E40"), Colour("#D891EF")
	};

	const std::array<Colour, 4> COLOUR_BIOMES = {
		Colour("#6e3529"), Colour("#934838"), Colour("#9e402c"), Colour("#656565")
	};
	const float COLOUR_SPREAD = 0.76f;
	const float COLOUR_HALF_SPREAD = COLOUR_SPREAD / 2.0f;
	const float COLOUR_AMPLITUDE = 16.0f;
	const float COLOUR_PART = 1.0f / (COLOUR_BIOMES.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, const Transform &transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_radius(radius),
		m_worldMatrix(transform.GetWorldMatrix()),
		m_randomColour(BRIGHT_COLOURS[(int)Maths::Random(0.0f, 20.0f)])
	{
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		/*if (m_radius == 0.0f)
		{*/
			return m_worldMatrix.Multiply(Vector3(x, 0.0f, z));
		/*}

		Vector4 cartesian = Vector4(x, 0.0f, z, 1.0f);
		cartesian = m_worldMatrix.Multiply(cartesian);
		cartesian = Vector3(cartesian).ProjectCubeToSphere(m_radius);

		Vector3 polar = Vector3(cartesian).CartesianToPolar();
		polar.m_x = Worlds::Get()->GetWorld()->GetTerrainRadius(m_radius, polar.m_y, polar.m_z);
		return polar.PolarToCartesian();*/
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		//Vector3 polar = position.CartesianToPolar();

		//float squareSize = 0.1f;
		//Vector3 positionL = GetPosition(polar.m_y - squareSize, polar.m_z);
		//Vector3 positionR = GetPosition(polar.m_y + squareSize, polar.m_z);
		//Vector3 positionD = GetPosition(polar.m_y, polar.m_z - squareSize);
		//Vector3 positionU = GetPosition(polar.m_y, polar.m_z + squareSize);

		//Vector3 normal = (positionL - positionR).Cross(positionD - positionU);
		////Vector3 normal = (positionL - positionR).Cross(positionR - positionD);
		//return normal.Normalize();

		//return position.ProjectCubeToSphere(m_radius);
		return Vector3::ZERO;
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		/*Vector3 polar = position.CartesianToPolar();
		float value = (polar.m_x - m_radius + COLOUR_AMPLITUDE) / (COLOUR_AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - COLOUR_HALF_SPREAD) * (1.0f / COLOUR_SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / COLOUR_PART));
		float blend = (value - (firstBiome * COLOUR_PART)) / COLOUR_PART;
		Colour colour = COLOUR_BIOMES.at(firstBiome).Interpolate(COLOUR_BIOMES.at(firstBiome + 1), blend);
		return colour;*/
		return m_randomColour;
	}
}
