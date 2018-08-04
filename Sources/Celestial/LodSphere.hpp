#pragma once

#include <memory>
#include <vector>
#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>

using namespace acid;

namespace test
{
	class LodSphere :
		public IComponent
	{
	private:
		std::vector<std::shared_ptr<Model>> m_lods;

		uint32_t m_latitudeBands;
		uint32_t m_longitudeBands;
		float m_radius;
	public:
		static const uint32_t HIGHEST_LOD;

		LodSphere(const uint32_t &latitudeBands = 30, const uint32_t &longitudeBands = 30, const float &radius = 1.0f);

		~LodSphere();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "LodSphere"; };

		uint32_t CalculateLod();
	};
}
