#pragma once

#include <Objects/IComponent.hpp>

using namespace acid;

namespace test
{
	class Gravity :
		public IComponent
	{
	private:
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
