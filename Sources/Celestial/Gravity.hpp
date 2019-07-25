#pragma once

#include <Scenes/Component.hpp>
#include "Celestial.hpp"

using namespace acid;

namespace micro
{
	class Gravity :
		public Component
	{
	public:
		Gravity();

		void Start() override;

		void Update() override;

		Celestial *GetStrongestInfluence() const { return m_influence; }

		friend const Metadata &operator>>(const Metadata &metadata, Gravity &gravity);

		friend Metadata &operator<<(Metadata &metadata, const Gravity &gravity);
	private:
		Celestial *m_influence{};
	};
}
