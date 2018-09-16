#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>

using namespace acid;

namespace micro
{
	class ICelestial :
		public IComponent
	{
	public:
		virtual float GetRadius() const = 0;

		virtual float GetMass() const = 0;
	};
}
