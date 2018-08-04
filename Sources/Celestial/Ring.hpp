#pragma once

#include <memory>
#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace test
{
	class Ring :
		public IComponent
	{
	private:
		static const float INNER_SCALE;
		static const float OUTER_SCALE;

		ICelestial *m_parent;

		float m_innerRadius;
		float m_outerRadius;
	public:
		Ring(ICelestial *parent = nullptr, const float &innerRadius = 900.0f, const float &outerRadius = 1300.0f);

		~Ring();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Ring"; };

		float GetInnerRadius() const { return m_innerRadius; }

		float GetOuterRadius() const { return m_outerRadius; }
	};
}
