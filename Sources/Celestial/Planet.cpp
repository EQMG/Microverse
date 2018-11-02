#include "Planet.hpp"

#include <Maths/Maths.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelRectangle.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Renderer/Pipelines/Compute.hpp>
#include <Helpers/FileSystem.hpp>
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"
#include "Star.hpp"

namespace micro
{
	const float Planet::MEDIAN_RADIUS = 700.0f; // +- 50.0%
	const float Planet::MEDIAN_DENSITY = 5510.0f;
	const float Planet::MEDIAN_MASS = MEDIAN_MASS * (4.0f / 3.0f) * PI * std::pow(MEDIAN_RADIUS, 3.0f);
	const float Planet::SQUARE_RADIUS_RATIO = 0.2f; // 0.2 or 0.4;

	Planet::Planet(const int &seed, const float &radius, const float &density, const float &axialTilt) :
		m_seed(seed),
		m_radius(radius),
		m_density(density),
		m_mass(m_density * (4.0f / 3.0f) * PI * std::pow(m_radius, 3.0f)),
		m_escapeVelocity(std::sqrt(2.0f * Star::G_CONSTANT * m_mass / m_radius)),
		m_axialTilt(axialTilt),
		m_axialTropics(axialTilt),
		m_axialPolar(90.0f - axialTilt),
		m_innerRings(1.34f * (m_radius / MEDIAN_RADIUS)),
		m_outterRings(2.44f * (m_radius / MEDIAN_RADIUS)),
		m_heightmap(std::vector<Colour>()),
		m_heightmapSize(3072)
	{
		float surfaceGravity = Star::G_CONSTANT * m_mass / std::pow(m_radius, 2.0f);

		Log::Out("Planet: Radius(m)=%f, Density(kg/m^3)=%f, Mass(kg)=%f, Surface Gravity(m/s^2)=%f, Escape Velocity(m/s)=%f\n", m_radius, m_density, m_mass, surfaceGravity, m_escapeVelocity);
	}

	void Planet::Start()
	{
		if (m_seed == -1)
		{
			auto mesh = GetGameObject()->GetComponent<Mesh>();

			if (mesh != nullptr)
			{
				mesh->SetModel(ModelSphere::Resource(32, 32, m_radius));
			//	mesh->SetModel(ModelObj::Resource("Icosphere.obj"));
			//	GetGameObject()->GetTransform().SetScaling(Vector3(m_radius, m_radius, m_radius));
			}

			return;
		}

		GenerateHeightmap();

		std::string baseName = GetGameObject()->GetName();
		float sideLength = 2.0f * m_radius;
		float squareSize = SQUARE_RADIUS_RATIO * m_radius;

		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, m_radius, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Top");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, -m_radius, 0.0f), Vector3(180.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Bottom");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, 0.0f, m_radius), Vector3(90.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Back");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(0.0f, 0.0f, -m_radius), Vector3(270.0f, 0.0f, 0.0f)), 0, sideLength, squareSize, "Front");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 270.0f)), 0, sideLength, squareSize, "Right");
		QuadtreeChunk::CreateChunk(this, Transform(Vector3(-m_radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 90.0f)), 0, sideLength, squareSize, "Left");
	}

	void Planet::Update()
	{
	}

	void Planet::Decode(const Metadata &metadata)
	{
	}

	void Planet::Encode(Metadata &metadata) const
	{
	}

	Colour Planet::GetColour(const Vector3 &cartesian)
	{
		//float u = ((std::atan2(cartesian.m_z / m_radius, cartesian.m_x / m_radius) / PI) + 1.0f) / 2.0f;
		//float v = 0.5f - (std::asin(cartesian.m_y / m_radius) / PI);

		return Colour::CLEAR; //  m_heightmap[uint32_t(u * m_heightmapSize) * (m_heightmapSize - 1) + uint32_t(v * m_heightmapSize)];
	}

	float Planet::GetRadius(const Vector3 &cartesian)
	{
		Colour heightmap = GetColour(cartesian);
		float height = 38.0f * ((2.0f * heightmap.m_r) - 1.0f);
		return m_radius + height;
	}

	void Planet::GenerateHeightmap()
	{
#if ACID_VERBOSE
		auto debugStart = Engine::GetTime();
#endif
		auto result = std::make_shared<Texture>(m_heightmapSize, m_heightmapSize);

		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_GRAPHICS_BIT);
		Compute compute = Compute(ComputeCreate("Shaders/Planet/Planet.comp", m_heightmapSize, m_heightmapSize, 32, {}));

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Uniform Object.
		auto uniformBlock = compute.GetShaderProgram()->GetUniformBlock("UboObject");
		UniformHandler uniform = UniformHandler(uniformBlock);
		uniform.Push("scale", 1.0f);
		uniform.Update(uniformBlock);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("UboObject", uniform);
		descriptorSet.Push("outColour", result);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer);
		compute.CmdRender(commandBuffer);

		commandBuffer.End();
		commandBuffer.Submit();

		uint8_t *pixels = result->GetPixels();
		test = result;

/*#if ACID_VERBOSE
		// Saves the planet texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/Planet.png";
		FileSystem::ClearFile(filename);
		Texture::WritePixels(filename, pixels, result->GetWidth(), result->GetHeight(), result->GetComponents());
#endif*/

		uint32_t pixelCount = result->GetWidth() * result->GetHeight();
		m_heightmap.reserve(pixelCount);

		for (uint32_t i = 0; i < pixelCount; i++)
		{
			uint8_t *pixelOffset = pixels + (i * result->GetComponents());
			float r = static_cast<float>(pixelOffset[0]) / 255.0f;
			float g = static_cast<float>(pixelOffset[1]) / 255.0f;
			float b = static_cast<float>(pixelOffset[2]) / 255.0f;
			float a = result->GetComponents() >= 4 ? static_cast<float>(pixelOffset[3]) / 255.0f : 0xff;

		//	m_heightmap[i] = Colour(r, g, b, a);
		}

		delete[] pixels;

#if ACID_VERBOSE
		auto debugEnd = Engine::GetTime();
		Log::Out("Computed planet in %ims\n", (debugEnd - debugStart).AsMilliseconds());
#endif
	}
}
