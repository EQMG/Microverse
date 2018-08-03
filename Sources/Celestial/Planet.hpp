#pragma once

#include <memory>
#include <Textures/Texture.hpp>
#include "Star.hpp"
#include "ICelestial.hpp"

using namespace acid;

namespace test
{
	class Planet :
		public ICelestial
	{
	private:
		Star *m_star;
		float m_radius;
		float m_density;
		float m_mass;
		float m_surfaceGravity;
		float m_escapeVelocity;
	public:
		static const float MEDIAN_RADIUS;
		static const float SQUARE_RADIUS_RATIO;

		Planet(Star *star = nullptr, const float &radius = 700.0f, const float &density = 5510.0f);

		~Planet();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Planet"; };

		GameObject *CreateChunk(const Transform &transform, const uint32_t &lod = 0, const float &sideLength = 100.0f, const float &squareSize = 100.0f, const std::string &namePostfix = "");

		float GetRadius() const { return m_radius; }

		float GetMass() const { return m_mass; }

		Star *GetStar() const { return m_star; }

		float GetDensity() const { return m_density; }
	private:
		std::shared_ptr<Texture> GenerateMap();
	};
}
