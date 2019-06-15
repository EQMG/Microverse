#pragma once

#include <Graphics/Renderer.hpp>

using namespace acid;

namespace micro
{
	class MainRenderer :
		public Renderer
	{
	public:
		MainRenderer();

		void Update() override;
	};
}
