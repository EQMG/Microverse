#pragma once

#include <Maths/Colour.hpp>
#include <Post/PostFilter.hpp>
#include <Maths/Visual/Driver.hpp>

using namespace acid;

namespace micro
{
	class FilterDamage :
		public PostFilter
	{
	public:
		explicit FilterDamage(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;

		const Colour &GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		void SetRadiusDriver(std::unique_ptr<Driver<float>> &&radiusDriver) { m_radiusDriver = std::move(radiusDriver); }

		const float &GetRadius() const { return m_radius; }

		void SetSoftnessDriver(std::unique_ptr<Driver<float>> &&softnessDriver) { m_softnessDriver = std::move(softnessDriver); }

		const float &GetSoftness() const { return m_softness; }
	private:
		PushHandler m_pushScene;

		Colour m_colour;

		std::unique_ptr<Driver<float>> m_radiusDriver;
		float m_radius{};

		std::unique_ptr<Driver<float>> m_softnessDriver;
		float m_softness{};
	};
}
