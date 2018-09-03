#pragma once

#include <Objects/IComponent.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace micro
{
	class Gravity :
		public IComponent
	{
	private:
		std::shared_ptr<ICelestial> m_influence;
	public:
		Gravity();

		~Gravity();

		void Start() override;

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;

		std::shared_ptr<ICelestial> GetStrongestInfluence() const { return m_influence; }
	};
}
