#include "Orbit.hpp"

#include <Objects/GameObject.hpp>

namespace test
{
	Orbit::Orbit(ICelestial *parent) :
		IComponent(),
		m_parent(parent)
	{
	}

	Orbit::~Orbit()
	{
	}

	void Orbit::Start()
	{
	}

	void Orbit::Update()
	{

	}

	void Orbit::Load(LoadedValue *value)
	{
	}

	void Orbit::Write(LoadedValue *destination)
	{
	}
}
