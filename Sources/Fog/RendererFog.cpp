#include "RendererFog.hpp"

#include <Scenes/Scenes.hpp>
#include <Models/VertexModel.hpp>

namespace acid
{
	RendererFog::RendererFog(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Fog/Fog.vert", "Shaders/Fog/Fog.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, {}))),
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

		//auto sceneFogRenders = Scenes::Get()->GetStructure()->QueryComponents<FogRender>();

		//for (auto &meshRender : sceneFogRenders)
		//{
		//	meshRender->CmdRender(commandBuffer, m_uniformScene);
		//}
	}
}
