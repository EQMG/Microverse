#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Helpers/FileSystem.hpp>
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
#include <Physics/ColliderConvexHull.hpp>
#include <Physics/ColliderSphere.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include "Celestial/Gravity.hpp"
#include "Celestial/LodSphere.hpp"
#include "Celestial/Orbit.hpp"
#include "Celestial/Materials/MaterialGasGiant.hpp"
#include "Celestial/Materials/MaterialRing.hpp"
#include "Celestial/Planet.hpp"
#include "Celestial/Ring.hpp"
#include "Celestial/Star.hpp"
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"
#include "Waters/MaterialWater.hpp"
#include "World/World.hpp"

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
		GetPhysics()->SetGravity(Vector3::ZERO);
		GetPhysics()->SetAirDensity(0.0f);

		// Camera.
		//GameObject *cameraObject = new GameObject(Transform(Vector3(), Vector3(), 1.0f));
		//cameraObject->SetName("Camera");
		//cameraObject->AddComponent<FpsCamera>();

		// Player.
		// GameObject *playerObject = new GameObject("Objects/Player/Player.json", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));
		GameObject *playerObject = new GameObject(Transform(Vector3(0.0f, 0.0f, 3000.0f), Vector3(0.0f, 180.0f, 0.0f), 1.0f));
		playerObject->SetName("Player");
		playerObject->AddComponent<FpsPlayer>();

		// Skybox.
		GameObject *skyboxObject1 = new GameObject("Objects/SkyboxWhite/SkyboxWhite.json", Transform(Vector3(), Vector3(), 2128.0f));
		GameObject *skyboxObject = new GameObject("Objects/SkyboxStars/SkyboxStars.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Animated.
	//	GameObject *animatedObject = new GameObject(Transform(Vector3(0.0f, 600.0f, 0.0f), Vector3(), 0.25f));
	//	animatedObject->SetName("Animated");
	//	animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
	//	animatedObject->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
	//	animatedObject->AddComponent<MeshRender>();
	//	animatedObject->AddComponent<ShadowRender>();

		// Entities.
		GameObject *star1 = new GameObject(Transform(Vector3()));
		star1->SetName("Star1");
		star1->AddComponent<Star>(1000.0f);
		star1->AddComponent<Light>(Colour::WHITE, -1.0f);
		star1->AddComponent<Mesh>();
		star1->AddComponent<LodSphere>(50, 50, 2000.0f);
		star1->AddComponent<MaterialDefault>(star1->GetComponent<Star>()->GetColour());
		star1->AddComponent<MeshRender>();

		GameObject *planet1 = new GameObject(Transform(Vector3(4000.0f, 0.0f, 5000.0f)));
		planet1->SetName("Planet1");
		planet1->SetParent(star1);
		planet1->AddComponent<Planet>(-1, 1500.0f, 1330.0f);
		planet1->AddComponent<Orbit>();
		planet1->AddComponent<Mesh>();
		planet1->AddComponent<LodSphere>(50, 50, 1500.0f);
		planet1->AddComponent<MaterialGasGiant>(0.32f, 0.0002f);
		planet1->AddComponent<MeshRender>();

		GameObject *rings1 = new GameObject(Transform());
		rings1->SetName("Planet1_Rings");
		rings1->SetParent(planet1);
		rings1->AddComponent<Mesh>();
		rings1->AddComponent<Ring>(2000.0f, 3300.0f);
		rings1->AddComponent<MaterialRing>();
		rings1->AddComponent<MeshRender>();

		GameObject *planet2 = new GameObject(Transform(Vector3(3000.0f, 0.0f, 0.0f)));
		planet2->SetName("Planet2");
		planet2->AddComponent<Planet>(5444, 200.0f);
		planet2->AddComponent<Orbit>();

		GameObject *planet3 = new GameObject(Transform(Vector3(4500.0f, 0.0f, 0.0f)));
		planet3->SetName("Planet1");
		planet3->SetParent(star1);
		planet3->AddComponent<Planet>(9784, 600.0f);
		planet3->AddComponent<Orbit>();

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
			GameObject *sphere = new GameObject(Transform(cameraPosition, Vector3(), 10.0f));
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
			sphere->AddComponent<ColliderSphere>();
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			sphere->AddComponent<Gravity>();
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			rigidbody->AddForce<Force>((cameraRotation.ToQuaternion() * Vector3::FRONT).Normalize() * -15.0f, 2.0f);
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
