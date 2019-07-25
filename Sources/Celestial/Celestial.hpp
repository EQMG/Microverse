#pragma once

#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>

using namespace acid;

namespace micro
{
	class Celestial :
		public Component
	{
	public:
		virtual const float &GetRadius() const = 0;

		virtual const float &GetMass() const = 0;

		static Vector3f ProjectCubeToSphere(const Vector3f &source, const float &radius)
		{
			if (radius == 0.0f)
			{
				return source;
			}

			auto cube{source / radius};
			auto dx{cube.m_x * cube.m_x};
			auto dy{cube.m_y * cube.m_y};
			auto dz{cube.m_z * cube.m_z};
			auto sx{cube.m_x * std::sqrt(1.0f - (dy / 2.0f) - (dz / 2.0f) + (dy * dz / 3.0f))};
			auto sy{cube.m_y * std::sqrt(1.0f - (dz / 2.0f) - (dx / 2.0f) + (dz * dx / 3.0f))};
			auto sz{cube.m_z * std::sqrt(1.0f - (dx / 2.0f) - (dy / 2.0f) + (dx * dy / 3.0f))};
			return {sx * radius, sy * radius, sz * radius};
		}
	};
}
