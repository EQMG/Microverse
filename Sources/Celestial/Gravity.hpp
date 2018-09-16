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
		ICelestial *m_influence;
	public:
		Gravity();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		ICelestial *GetStrongestInfluence() const { return m_influence; }
	};
}
