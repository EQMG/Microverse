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
#include <Models/Shapes/ShapeSphere.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include "Terrains/LodBehaviour.hpp"
#include "Terrains/MeshTerrain.hpp"
#include "Terrains/MaterialTerrain.hpp"
#include "Voxels/MaterialVoxel.hpp"
#include "Voxels/VoxelChunk.hpp"
#include "Waters/MaterialWater.hpp"
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"

namespace test
{
	static const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(new FpsCamera()),
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
		m_uiStartLogo->SetAlphaDriver(new DriverConstant(1.0f));
		m_overlayDebug->SetAlphaDriver(new DriverConstant(0.0f));
		m_uiNavigation->SetAlphaDriver(new DriverConstant(0.0f));
	}

	Scene1::~Scene1()
	{
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
		GameObject *playerObject = new GameObject(Transform(Vector3(0.0f, 256.0f, 512.0f), Vector3(0.0f, 180.0f, 0.0f), 1.0f));
		playerObject->SetName("Player");
		playerObject->AddComponent<FpsPlayer>();

		// Skybox.
		// GameObject *skyboxObject = new GameObject("Objects/SkyboxClouds/SkyboxStars.json", Transform(Vector3(), Vector3(), 2048.0f));
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 2048.0f));
		skyboxObject->SetName("SkyboxStars");
		skyboxObject->AddComponent<Mesh>(ShapeSphere::Resource(6, 6, 1.0f));
		skyboxObject->AddComponent<MaterialSkybox>(Cubemap::Resource("Objects/SkyboxStars", ".png"), false);
		skyboxObject->AddComponent<MeshRender>();

		// Entities.
		GameObject *sun = new GameObject(Transform(Vector3(100.0f, 1000.0f, 8000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour("#FFFFFF"), -1.0f);

		// Terrains.
		int n = 2;
		float side = MeshTerrain::SIDE_LENGTH;
		float radius = ((2 * n + 1) * side) / 2.0f;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				CreateChunk(radius, Transform(Vector3(j * side, radius, w * side), Vector3(0.0f, 0.0f, 0.0f))); // Top.
				CreateChunk(radius, Transform(Vector3(j * side, -radius, w * side), Vector3(180.0f, 0.0f, 0.0f))); // Bottom.
				CreateChunk(radius, Transform(Vector3(w * side, j * side, radius), Vector3(90.0f, 0.0f, 0.0f))); // Back.
				CreateChunk(radius, Transform(Vector3(w * side, j * side, -radius), Vector3(270.0f, 0.0f, 0.0f))); // Front.
				CreateChunk(radius, Transform(Vector3(radius, j * side, w * side), Vector3(0.0f, 0.0f, 270.0f))); // Right.
				CreateChunk(radius, Transform(Vector3(-radius, j * side, w * side), Vector3(0.0f, 0.0f, 90.0f))); // Left.
			}
		}

		// Voxels.
		/*GameObject *voxelChunk = new GameObject(Transform());
		voxelChunk->SetName("Chunk_0_0");
		voxelChunk->AddComponent<Mesh>();
		voxelChunk->AddComponent<MaterialVoxel>();
		voxelChunk->AddComponent<VoxelChunk>(MESH_GREEDY, true);
		voxelChunk->AddComponent<MeshRender>();*/

		// Waters.
		/*GameObject *water = new GameObject(Transform());
		water->SetName("Water");
		water->AddComponent<Mesh>();
		water->AddComponent<MaterialWater>();
		water->AddComponent<MeshRender>();*/
	}

	void Scene1::Update()
	{
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
			m_uiStartLogo->SetAlphaDriver(new DriverConstant(0.0f));
			m_overlayDebug->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, UI_SLIDE_TIME));
			//m_uiNavigation->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, UI_SLIDE_TIME));
			m_uiStartLogo->SetStarting(false);
		}
	}

	bool Scene1::IsGamePaused()
	{
		return m_uiStartLogo->IsStarting() || m_uiNavigation->GetAlpha() != 0.0f;
	}

	GameObject *Scene1::CreateChunk(const float &radius, const Transform &transform)
	{
		new GameObject("Objects/PlanetCentre/PlanetCentre.json", Transform(transform.GetPosition().ProjectCubeToSphere(radius), transform.GetRotation(), 5.0f));

		GameObject *terrainChunk = new GameObject(Transform());
		terrainChunk->SetName("Terrain");
		terrainChunk->AddComponent<Mesh>();
		terrainChunk->AddComponent<LodBehaviour>(radius, transform);
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
			m_uiNavigation->SetAlphaDriver(new DriverSlide(m_uiNavigation->GetAlpha(), 0.0f, UI_SLIDE_TIME));
		}
		else
		{
			m_uiNavigation->SetAlphaDriver(new DriverSlide(m_uiNavigation->GetAlpha(), 1.0f, UI_SLIDE_TIME));
		}
	}
}
