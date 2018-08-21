#pragma once

#include <Renderer/Buffers/UniformBuffer.hpp>
#include <Renderer/IRenderer.hpp>
#include <Renderer/Handlers/UniformHandler.hpp>
#include <Renderer/Pipelines/Pipeline.hpp>

using namespace acid;

namespace test
{
	class RendererFog :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		UniformHandler m_uniformScene;
	public:
		RendererFog(const GraphicsStage &graphicsStage);

		~RendererFog();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
