#include "FilterDamage.hpp"

#include <Maths/Visual/DriverConstant.hpp>

namespace micro
{
	FilterDamage::FilterDamage(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Damage.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_colour(Colour(0.0f, 0.0f, 0.0f, 0.0f)),
		m_radiusDriver(std::make_shared<DriverConstant>(0.0f)),
		m_radius(0.0f),
		m_softnessDriver(std::make_shared<DriverConstant>(0.0f)),
		m_softness(0.0f)
	{
		m_radius = 0.1f;
		m_colour = Colour::RED;
	}

	FilterDamage::~FilterDamage()
	{
	}

	void FilterDamage::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		auto delta = Engine::Get()->GetDeltaRender();
		m_radius = m_radiusDriver->Update(delta);
		m_softness = m_softnessDriver->Update(delta);

		if (m_radius == 0.0f || m_colour.m_a == 0.0f)
		{
			return;
		}

		// Updates uniforms.
		m_uniformScene.Push("colour", m_colour);
		m_uniformScene.Push("radius", m_radius);
		m_uniformScene.Push("softness", m_softness);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(5));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(5));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
