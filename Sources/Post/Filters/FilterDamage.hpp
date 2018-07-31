#pragma once

#include <Maths/Colour.hpp>
#include <Post/IPostFilter.hpp>
#include <Maths/Visual/IDriver.hpp>

namespace acid
{
	class FilterDamage :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		Colour m_colour;

		std::shared_ptr<IDriver> m_radiusDriver;
		float m_radius;

		std::shared_ptr<IDriver> m_softnessDriver;
		float m_softness;
	public:
		FilterDamage(const GraphicsStage &graphicsStage);

		~FilterDamage();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Colour GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		void SetRadiusDriver(std::shared_ptr<IDriver> radiusDriver) { m_radiusDriver = radiusDriver; }

		template<typename T, typename... Args>
		void SetRadiusDriver(Args &&... args) { SetRadiusDriver(std::make_shared<T>(std::forward<Args>(args)...)); }

		float GetRadius() const { return m_radius; }

		void SetSoftnessDriver(std::shared_ptr<IDriver> softnessDriver) { m_softnessDriver = softnessDriver; }

		template<typename T, typename... Args>
		void SetSoftnessDriver(Args &&... args) { SetSoftnessDriver(std::make_shared<T>(std::forward<Args>(args)...)); }

		float GetSoftness() const { return m_softness; }
	};
}
