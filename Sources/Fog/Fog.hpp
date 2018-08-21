#pragma once

#include <Objects/IComponent.hpp>
#include <Renderer/Handlers/DescriptorsHandler.hpp>
#include <Renderer/Handlers/UniformHandler.hpp>
#include <Renderer/Pipelines/Pipeline.hpp>
#include <Models/Model.hpp>

using namespace acid;

namespace test
{
	class Fog :
		public IComponent
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;
		std::shared_ptr<Model> m_sphere;

		float m_thickness;
		float m_innerRadius;
		float m_outerRadius;
	public:
		Fog(const float &thickness = 1.3f);

		~Fog();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline &pipeline);

		UniformHandler GetUniformObject() const { return m_uniformObject; }

		bool operator<(const Fog &other) const;
	};
}
