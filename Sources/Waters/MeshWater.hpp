#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Colour.hpp>

using namespace acid;

namespace test
{
	class MeshWater :
		public MeshSimple
	{
	public:
		static const float SIDE_LENGTH;
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;
		static const float TEXTURE_SCALE;
		static const Colour WATER_COLOUR;

		MeshWater();

		~MeshWater();

		MeshSimpleVertex CalculateVertex(const float &x, const float &z) override;
	};
}