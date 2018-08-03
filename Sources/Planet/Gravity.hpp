#pragma once

#include <memory>
#include <Objects/IComponent.hpp>
#include <Physics/Force.hpp>

using namespace acid;

namespace test
{
	class Gravity :
		public IComponent
	{
	private:
		std::shared_ptr<Force> m_force;
	public:
		Gravity();

		~Gravity();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Gravity"; };
	};
}
