#pragma once

#include <Maths/Colour.hpp>
#include <Post/IPostFilter.hpp>
#include <Maths/Visual/IDriver.hpp>

namespace acid
{
	class FL_EXPORT FilterDamage :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		Colour m_colour;

		IDriver *m_radiusDriver;
		float m_radius;

		IDriver *m_softnessDriver;
		float m_softness;
	public:
		FilterDamage(const GraphicsStage &graphicsStage);

		~FilterDamage();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Colour GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		void SetRadiusDriver(IDriver *radiusDriver);

		float GetRadius() const { return m_radius; }

		void SetSoftnessDriver(IDriver *softnessDriver);

		float GetSoftness() const { return m_softness; }
	};
}
