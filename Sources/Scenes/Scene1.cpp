#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include <Textures/stb_image_write.h>
#include <Helpers/FileSystem.hpp>
#include <Scenes/Scenes.hpp>
#include <Physics/ColliderSphere.hpp>
#include "World/World.hpp"
#include "Terrains/LodBehaviour.hpp"
#include "Terrains/MeshTerrain.hpp"
#include "Terrains/MaterialTerrain.hpp"
#include "Waters/MaterialWater.hpp"
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"

namespace test
{
	static const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(new FpsCamera()),
		m_buttonSpawnSphere(new ButtonMouse({MOUSE_BUTTON_1})),
		m_buttonFullscreen(new ButtonKeyboard({KEY_F11})),
		m_buttonCaptureMouse(new ButtonKeyboard({KEY_M})),
		m_buttonScreenshot(new ButtonKeyboard({KEY_F12})),
		m_buttonExit(new ButtonKeyboard({KEY_DELETE})),
		m_soundScreenshot(new Sound("Sounds/Screenshot.ogg")),
		m_primaryColour(new Colour("#e74c3c")),
		m_selectorJoystick(new SelectorJoystick(JOYSTICK_1, 0, 1, 0, 1)),
		m_buttonPause((new ButtonCompound({
			new ButtonKeyboard({KEY_ESCAPE}),
			new ButtonJoystick(JOYSTICK_1, {7})
		}))),
		m_uiStartLogo(new UiStartLogo(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer())),
		m_uiNavigation(new UiNavigation(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant>(0.0f);
		m_uiNavigation->SetAlphaDriver<DriverConstant>(0.0f);
	}

	Scene1::~Scene1()
	{
		delete m_buttonSpawnSphere;
		delete m_buttonFullscreen;
		delete m_buttonCaptureMouse;
		delete m_buttonScreenshot;
		delete m_buttonExit;

		delete m_primaryColour;
		delete m_selectorJoystick;

		delete m_buttonPause;
		delete m_uiStartLogo;
		delete m_overlayDebug;
		delete m_uiNavigation;
	}

	void Scene1::Start()
	{
		// Camera.
		//GameObject *cameraObject = new GameObject(Transform(Vector3(), Vector3(), 1.0f));
		//cameraObject->SetName("Camera");
		//cameraObject->AddComponent<FpsCamera>();

		// Player.
		// GameObject *playerObject = new GameObject("Objects/Player/Player.json", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));
		GameObject *playerObject = new GameObject(Transform(Vector3(0.0f, 0.0f, 2000.0f), Vector3(0.0f, 180.0f, 0.0f), 1.0f));
		playerObject->SetName("Player");
		playerObject->AddComponent<FpsPlayer>();

		// Skybox.
		GameObject *skyboxObject1 = new GameObject("Objects/SkyboxChapel/SkyboxChapel.json", Transform(Vector3(), Vector3(), 2000.0f));

		// Skybox.
		// GameObject *skyboxObject = new GameObject("Objects/SkyboxClouds/SkyboxStars.json", Transform(Vector3(), Vector3(), 2048.0f));
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 2048.0f));
		skyboxObject->SetName("SkyboxStars");
		skyboxObject->AddComponent<Mesh>(ModelSphere::Resource(6, 6, 1.0f));
		skyboxObject->AddComponent<MaterialSkybox>(Cubemap::Resource("Objects/SkyboxStars", ".png"));
		skyboxObject->AddComponent<MeshRender>();

		// Animated.
		GameObject *animatedObject = new GameObject(Transform(Vector3(0.0f, 500.0f, 0.0f), Vector3(), 0.25f));
		animatedObject->SetName("Animated");
		animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
		animatedObject->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
		animatedObject->AddComponent<MeshRender>();
	//	animatedObject->AddComponent<ShadowRender>();

		// Entities.
		GameObject *sun = new GameObject(Transform(Vector3(100.0f, 8000.0f, 100.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour::WHITE, -1.0f);

		// Terrains.
		GameObject *planet1 = CreatePlanet(500.0f, Vector3());

		// Waters.
		/*GameObject *water = new GameObject(Transform());
		water->SetName("Water");
		water->AddComponent<Mesh>();
		water->AddComponent<MaterialWater>();
		water->AddComponent<MeshRender>();*/
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere->WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
			GameObject *sphere = new GameObject(Transform(cameraPosition, Vector3(), 0.5f));
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
			sphere->AddComponent<ColliderSphere>();
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			rigidbody->AddForce<Force>((cameraRotation.ToQuaternion() * Vector3::FRONT) * -3.0f, 2.0f);
		}

		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse->WasDown())
		{
			//	Scenes::Get()->SetScene(new Scene2());
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonScreenshot->WasDown())
		{
			std::string filename = "Screenshots/" + Engine::Get()->GetDateTime() + ".png";

			m_soundScreenshot->Play();
			Screenshot::Capture(filename);
		}

		if (m_buttonExit->WasDown())
		{
			Engine::Get()->RequestClose(false);
		}

		if (m_buttonPause->WasDown())
		{
			TogglePause();
		}

		if (m_uiStartLogo->GetAlpha() == 0.0f && m_uiStartLogo->IsStarting())
		{
			m_uiStartLogo->SetAlphaDriver<DriverConstant>(0.0f);
			m_overlayDebug->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, UI_SLIDE_TIME);
		//	m_uiNavigation->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, UI_SLIDE_TIME);
			m_uiStartLogo->SetStarting(false);
		}
	}

	bool Scene1::IsGamePaused()
	{
		return m_uiStartLogo->IsStarting() || m_uiNavigation->GetAlpha() != 0.0f;
	}

	GameObject *Scene1::CreatePlanet(const float &radius, const Vector3 &position)
	{
		/*{
			Noise noiseTerrain = Noise(69124);
			noiseTerrain.SetNoiseType(NoiseType::TYPE_PERLINFRACTAL);
			noiseTerrain.SetFrequency(0.00625f);
			noiseTerrain.SetInterp(NoiseInterp::INTERP_QUINTIC);
			noiseTerrain.SetFractalType(NoiseFractal::FRACTAL_FBM);
			noiseTerrain.SetFractalOctaves(4);
			noiseTerrain.SetFractalLacunarity(1.8f);
			noiseTerrain.SetFractalGain(0.6f);

			Noise noiseBiome = Noise(56744);
			noiseBiome.SetNoiseType(NoiseType::TYPE_SIMPLEXFRACTAL);
			noiseBiome.SetFrequency(0.001f);
			noiseBiome.SetInterp(NoiseInterp::INTERP_QUINTIC);
			noiseBiome.SetFractalType(NoiseFractal::FRACTAL_FBM);
			noiseBiome.SetFractalOctaves(2);
			noiseBiome.SetFractalLacunarity(2.0f);
			noiseBiome.SetFractalGain(0.5f);

			std::string filename = FileSystem::GetWorkingDirectory() + "/Planet1.png";
			int width = 2500;
			int height = 2500;
			unsigned int *data = (unsigned int *) malloc(width * height * 4);

			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					float random = (noiseTerrain.GetNoise(x, y) + 1.0f) / 2.0f;
					float r = random * ((noiseBiome.GetNoise(x, y) + 1.0f) / 2.0f);
					float g = random * ((noiseBiome.GetNoise(x * 10.0f, y * 10.0f) + 1.0f) / 2.0f);
					float b = random * ((noiseBiome.GetNoise(x * 33.0f, y * 33.0f) + 1.0f) / 2.0f);

					unsigned char buffer[4];
					buffer[0] = std::floor(255 * Maths::Clamp(r, 0.0f, 1.0f));
					buffer[1] = std::floor(255 * Maths::Clamp(g, 0.0f, 1.0f));
					buffer[2] = std::floor(255 * Maths::Clamp(b, 0.0f, 1.0f));
					buffer[3] = 255;
					memcpy((char*)&data[x * width + y], buffer, 4);
				}
			}

			FileSystem::DeleteFile(filename);
			Texture::WritePixels(filename, data, width, height, 4);
			free(data);
		}*/

		//	GameObject *planet = new GameObject("Objects/PlanetCentre/PlanetCentre.json", Transform(position, Vector3(), 5.0f));
		GameObject *planet = new GameObject(Transform(position));
		planet->AddComponent<ColliderSphere>(radius);
		planet->AddComponent<Rigidbody>(0.0f);

		GameObject *chunkTop = CreateChunk(radius, Transform(Vector3(0.0f, radius, 0.0f), Vector3(0.0f, 0.0f, 0.0f)));
		chunkTop->SetName("Planet_Top");
		chunkTop->SetParent(planet);
		GameObject *chunkBottom = CreateChunk(radius, Transform(Vector3(0.0f, -radius, 0.0f), Vector3(180.0f, 0.0f, 0.0f)));
		chunkBottom->SetName("Planet_Bottom");
		chunkBottom->SetParent(planet);
		GameObject *chunkBack = CreateChunk(radius, Transform(Vector3(0.0f, 0.0f, radius), Vector3(90.0f, 0.0f, 0.0f)));
		chunkBack->SetName("Planet_Back");
		chunkBack->SetParent(planet);
		GameObject *chunkFront = CreateChunk(radius, Transform(Vector3(0.0f, 0.0f, -radius), Vector3(270.0f, 0.0f, 0.0f)));
		chunkFront->SetName("Planet_Front");
		chunkFront->SetParent(planet);
		GameObject *chunkRight = CreateChunk(radius, Transform(Vector3(radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 270.0f)));
		chunkRight->SetName("Planet_Right");
		chunkRight->SetParent(planet);
		GameObject *chunkLeft = CreateChunk(radius, Transform(Vector3(-radius, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 90.0f)));
		chunkLeft->SetName("Planet_Left");
		chunkLeft->SetParent(planet);

		return planet;
	}

	GameObject *Scene1::CreateChunk(const float &radius, const Transform &transform)
	{
	//	new GameObject("Objects/PlanetCentre/PlanetCentre.json", Transform(transform.GetPosition().ProjectCubeToSphere(radius), transform.GetRotation(), 5.0f));

		GameObject *terrainChunk = new GameObject(Transform());
		terrainChunk->AddComponent<Mesh>();
		terrainChunk->AddComponent<LodBehaviour>(0, 2.0f * radius, radius, radius * LodBehaviour::GetSideRadiusRatio(radius), transform);
		terrainChunk->AddComponent<MaterialTerrain>();
		terrainChunk->AddComponent<MeshRender>();
		//terrainChunk->AddComponent<ShadowRender>();
		return terrainChunk;
	}

	void Scene1::TogglePause()
	{
		if (m_uiStartLogo->IsStarting())
		{
			return;
		}

		if (IsGamePaused())
		{
			m_uiNavigation->SetAlphaDriver<DriverSlide>(m_uiNavigation->GetAlpha(), 0.0f, UI_SLIDE_TIME);
		}
		else
		{
			m_uiNavigation->SetAlphaDriver<DriverSlide>(m_uiNavigation->GetAlpha(), 1.0f, UI_SLIDE_TIME);
		}
	}
}
