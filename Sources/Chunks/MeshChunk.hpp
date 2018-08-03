#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Maths.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Transform.hpp>

using namespace acid;

namespace test
{
	class MeshChunk :
		public MeshSimple
	{
	private:
		float m_radius;
		Matrix4 m_worldMatrix;
	public:
		MeshChunk(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, const Transform &transform);

		~MeshChunk();

		virtual Vector3 GetPosition(const float &x, const float &z) override;

		virtual Vector3 GetNormal(const float &x, const float &z, const Vector3 &position) override;

		virtual Vector3 GetColour(const Vector3 &position, const Vector3 &normal) override;
	};
}