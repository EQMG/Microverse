#include "MeshChunk.hpp"

#include "World/World.hpp"

namespace micro
{
	const std::array<Colour, 4> COLOUR_BIOMES{
		"#6e3529",
		"#934838",
		"#9e402c",
		"#656565"
	};
	const float COLOUR_SPREAD{0.76f};
	const float COLOUR_HALF_SPREAD{COLOUR_SPREAD / 2.0f};
	const float COLOUR_AMPLITUDE{28.0f};
	const float COLOUR_PART{1.0f / (COLOUR_BIOMES.size() - 1)};

	MeshChunk::MeshChunk(Planet *planet, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale, const Transform &transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_parent(planet),
		m_worldMatrix(transform.GetWorldMatrix()),
		m_edgeConditions(East | South)
	{
		GenerateMesh();
	}

	VertexDefault MeshChunk::GetVertex(const uint32_t &col, const uint32_t &row)
	{
		auto x{((row * m_squareSize) - m_sideLength) / 2.0f};
		auto z{((col * m_squareSize) - m_sideLength) / 2.0f};

		Vector3f cartesian{m_worldMatrix.Multiply({x, 0.0f, z, 1.0f})};
		cartesian = ProjectCubeToSphere(cartesian, m_parent->GetRadius());

		auto polar{cartesian.CartesianToPolar()};
		polar.m_x = m_parent->GetRadius(cartesian);
		auto position{polar.PolarToCartesian()};

		Vector2f uv{((std::atan2(cartesian.m_z / m_parent->GetRadius(), cartesian.m_x / m_parent->GetRadius()) / Maths::Pi<float>) + 1.0f) / 2.0f,
			0.5f - (std::asin(cartesian.m_y / m_parent->GetRadius()) / Maths::Pi<float>)
		}; // Vector2f::Zero
		auto normal{GetNormal(x, z)}; // Vector3f::Zero
		//auto colour{m_parent->GetColour(cartesian)}; // Colour::White
		return {position, uv, normal};
	}

	Vector3f MeshChunk::GetPosition(const float &x, const float &z)
	{
		Vector3f cartesian{m_worldMatrix.Multiply({x, 0.0f, z, 1.0f})};

		if (m_parent->GetRadius() == 0.0f)
		{
			return cartesian;
		}

		cartesian = ProjectCubeToSphere(cartesian, m_parent->GetRadius());
		auto polar{cartesian.CartesianToPolar()};
		polar.m_x = m_parent->GetRadius(cartesian);
		return polar.PolarToCartesian();
	}

	Vector3f MeshChunk::GetNormal(const float &x, const float &z)
	{
		auto positionL{GetPosition(x - 1.0f, z)};
		auto positionR{GetPosition(x + 1.0f, z)};
		auto positionD{GetPosition(x, z - 1.0f)};
		//auto positionU{GetPosition(x, z + 1.0f)};

		auto normal{(positionL - positionR).Cross(positionR - positionD)};
		return normal.Normalize();
	}

	Vector3f MeshChunk::ProjectCubeToSphere(const Vector3f &source, const float &radius)
	{
		if (radius == 0.0f)
		{
			return source;
		}

		Vector3 cube{source / radius};
		auto dx{cube.m_x * cube.m_x};
		auto dy{cube.m_y * cube.m_y};
		auto dz{cube.m_z * cube.m_z};
		auto sx{cube.m_x * std::sqrt(1.0f - (dy / 2.0f) - (dz / 2.0f) + (dy * dz / 3.0f))};
		auto sy{cube.m_y * std::sqrt(1.0f - (dz / 2.0f) - (dx / 2.0f) + (dz * dx / 3.0f))};
		auto sz{cube.m_z * std::sqrt(1.0f - (dx / 2.0f) - (dy / 2.0f) + (dx * dy / 3.0f))};
		return {sx * radius, sy * radius, sz * radius};
	}
}
