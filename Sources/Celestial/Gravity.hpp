#pragma once

#include <Objects/IComponent.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace test
{
	class Gravity :
		public IComponent
	{
	private:
		ICelestial *m_influence;
	public:
		Gravity();

		~Gravity();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Gravity"; };

		ICelestial *GetStrongestInfluence() const { return m_influence; }
	};
}
