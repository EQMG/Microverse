#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>

using namespace acid;

namespace test
{
	class ICelestial :
		public IComponent
	{
	public:
		ICelestial() :
			IComponent()
		{
		}

		~ICelestial()
		{
		}

		virtual void Start() override = 0;

		virtual void Update() override = 0;

		virtual void Load(LoadedValue *value) override = 0;

		virtual void Write(LoadedValue *destination) override = 0;

		virtual float GetRadius() const = 0;

		virtual float GetMass() const = 0;
	};
}
