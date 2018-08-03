#pragma once

#include <Objects/IComponent.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace test
{
	class Orbit :
		public IComponent
	{
	private:
		ICelestial *m_parent;
	public:
		Orbit(ICelestial *parent = nullptr);

		~Orbit();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Orbit"; };

		ICelestial *GetParent() const { return m_parent; }
	};
}
