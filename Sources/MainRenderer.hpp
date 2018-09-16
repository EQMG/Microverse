#pragma once

#include <Renderer/IManagerRender.hpp>

using namespace acid;

namespace micro
{
	class MainRenderer :
		public IManagerRender
	{
	public:
		MainRenderer();

		void Start() override;

		void Update() override;
	};
}
