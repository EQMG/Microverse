#pragma once

#include <memory>
#include <vector>
#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>
#include "ICelestial.hpp"

using namespace acid;

namespace test
{
	class LodSphere :
		public IComponent
	{
	private:
		ICelestial *m_parent;

		std::vector<std::shared_ptr<Model>> m_lods;

		unsigned int m_latitudeBands;
		unsigned int m_longitudeBands;
		float m_radius;
	public:
		static const uint32_t HIGHEST_LOD;

		LodSphere(ICelestial *parent = nullptr, const unsigned int &latitudeBands = 30, const unsigned int &longitudeBands = 30, const float &radius = 1.0f);

		~LodSphere();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "LodSphere"; };

		uint32_t CalculateLod();
	};
}
