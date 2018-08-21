#include "RendererFog.hpp"

#include <algorithm>
#include <Scenes/Scenes.hpp>
#include <Models/VertexModel.hpp>
#include "Fog.hpp"

namespace test
{
	RendererFog::RendererFog(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Fogs/Fog.vert", "Shaders/Fogs/Fog.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {}))),
		m_uniformScene(UniformHandler(true))
	{
	}

	RendererFog::~RendererFog()
	{
	}

	void RendererFog::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());
		m_uniformScene.Push("cameraPos", camera.GetPosition());

		auto sceneFogs = Scenes::Get()->GetStructure()->QueryComponents<Fog>();
		std::sort(sceneFogs.begin(), sceneFogs.end());

		for (auto &fog : sceneFogs)
		{
			fog->CmdRender(commandBuffer, m_uniformScene, m_pipeline);
		}
	}
}
