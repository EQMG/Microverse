#pragma once

#include <memory>
#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>
#include <Physics/Force.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class Planet :
		public IComponent
	{
	private:
		float m_radius;
		float m_density;
		float m_mass;
		float m_surfaceGravity;
	public:
		static const float SQUARE_RADIUS_RATIO;
		static const float G_CONSTANT;

		Planet(const float &radius = 500.0f, const float &density = 300000.0f);

		~Planet();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		GameObject *CreateChunk(const Transform &transform, const uint32_t &lod = 0, const float &sideLength = 100.0f, const float &squareSize = 100.0f, const std::string &namePostfix = "");

		std::string GetName() const override { return "Planet"; };

		float GetRadius() const { return m_radius; }

		float GetDensity() const { return m_density; }

		float GetMass() const { return m_mass; }
	private:
		std::shared_ptr<Texture> GenerateMap();
	};
}
