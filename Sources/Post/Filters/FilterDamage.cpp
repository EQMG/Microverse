#include "FilterDamage.hpp"

#include <Maths/Visual/DriverConstant.hpp>

namespace micro
{
	FilterDamage::FilterDamage(const Pipeline::Stage &pipelineStage) :
		PostFilter{pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Damage.frag"}},
		m_colour{Colour::Red},
		m_radiusDriver{std::make_unique<DriverConstant<float>>(0.0f)},
		m_softnessDriver{std::make_unique<DriverConstant<float>>(0.0f)}
	{
	}

	void FilterDamage::Render(const CommandBuffer &commandBuffer)
	{
		auto delta{Engine::Get()->GetDeltaRender()};
		m_radius = m_radiusDriver->Update(delta);
		m_softness = m_softnessDriver->Update(delta);

		if (m_radius == 0.0f || m_colour.m_a == 0.0f)
		{
			return;
		}

		// Updates uniforms.
		m_pushScene.Push("colour", m_colour);
		m_pushScene.Push("radius", m_radius);
		m_pushScene.Push("softness", m_softness);

		// Updates descriptors.
		m_descriptorSet.Push("PushScene", m_pushScene);
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

		if (!m_descriptorSet.Update(m_pipeline))
		{
			return;
		}

		// Binds the pipeline.
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		m_pushScene.BindPush(commandBuffer, m_pipeline);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
}
