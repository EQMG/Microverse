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
		ICelestial() :
			IComponent()
		{
		}

		~ICelestial()
		{
		}

		virtual void Start() override = 0;

		virtual void Update() override = 0;

		virtual void Decode(const Node &node) override = 0;

		virtual void Encode(Node &node) const override = 0;

		virtual float GetRadius() const = 0;

		virtual float GetMass() const = 0;
	};
}
