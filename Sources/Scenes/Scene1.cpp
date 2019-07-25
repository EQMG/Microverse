#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>
#include <Physics/Colliders/ColliderSphere.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include "Celestial/Gravity.hpp"
//#include "Celestial/Orbit.hpp"
//#include "Celestial/Fogs/MaterialFog.hpp"
//#include "Celestial/GasGiants/MaterialGasGiant.hpp"
//#include "Celestial/Rings/MaterialRing.hpp"
#include "Celestial/Planet.hpp"
#include "Celestial/Star.hpp"
#include "CameraFree.hpp"
#include "World/World.hpp"

namespace micro
{
	static const Time UI_SLIDE_TIME{0.2s};

	Scene1::Scene1() :
		Scene{std::make_unique<CameraFree>()},
		m_buttonSpawnSphere{std::make_unique<ButtonMouse>(MouseButton::Left)},
		m_buttonCaptureMouse{std::make_unique<ButtonKeyboard>(Key::Escape), std::make_unique<ButtonKeyboard>(Key::M)},
		m_buttonSave{Key::K},
		m_uiStartLogo{&Uis::Get()->GetCanvas()},
		m_overlayDebug{&Uis::Get()->GetCanvas()}
	{
		m_buttonSpawnSphere.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
		{
			if (action == InputAction::Press)
			{
				auto cameraPosition{Scenes::Get()->GetCamera()->GetPosition()};
				auto cameraRotation{Scenes::Get()->GetCamera()->GetRotation()};

				auto sphere = GetStructure()->CreateEntity({cameraPosition, {}, 100.0f});
				sphere->AddComponent<Mesh>(ModelSphere::Create(0.5f, 32, 32));
				sphere->AddComponent<Gravity>();
				auto rigidbody = sphere->AddComponent<Rigidbody>(100.0f);
				rigidbody->AddForce(std::make_unique<Force>(-15000.0f * (Quaternion{cameraRotation} * Vector3f::Front).Normalize(), 2s));
				sphere->AddComponent<ColliderSphere>();
				//sphere->AddComponent<ColliderSphere>(0.5f, Transform{{0.0f, 1.0f, 0.0f}});
				sphere->AddComponent<MaterialDefault>(Colour::White, nullptr, 0.0f, 1.0f);
				sphere->AddComponent<MeshRender>();
				sphere->AddComponent<ShadowRender>();
			}
		});

		m_buttonCaptureMouse.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
		{
			if (action == InputAction::Press)
			{
				Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
			}
		});

		m_uiStartLogo.SetAlphaDriver(std::make_unique<DriverConstant<float>>(1.0f));
		m_overlayDebug.SetAlphaDriver(std::make_unique<DriverConstant<float>>(0.0f));

		m_uiStartLogo.OnFinished().Add([this]()
		{
			m_overlayDebug.SetAlphaDriver(std::make_unique<DriverSlide<float>>(0.0f, 1.0f, UI_SLIDE_TIME));
			Mouse::Get()->SetCursorHidden(true);
		});
	}

	void Scene1::Start()
	{
		GetPhysics()->SetGravity({});
		GetPhysics()->SetAirDensity(0.0f);

		// Skybox.
		auto skyboxObject{GetStructure()->CreateEntity({{}, {}, 2048.0f}, "Objects/SkyboxClouds/SkyboxClouds.json")};
		
		// Entities.
		auto star1{GetStructure()->CreateEntity({{0.0f, 0.0f, 0.0f}})};
		star1->SetName("Star1");
		star1->AddComponent<Star>(1000.0f);
		star1->AddComponent<Light>(star1->GetComponent<Star>()->GetColour(), -1.0f);
		star1->AddComponent<Mesh>();
		star1->AddComponent<MaterialDefault>(star1->GetComponent<Star>()->GetColour(), nullptr, 0.0f, 0.0f, nullptr, nullptr, false, true, true);
		star1->AddComponent<MeshRender>();

		/*auto planet1{GetStructure()->CreateEntity({{0.0f, 0.0f, 5000.0f}})};
		planet1->SetName("Planet1");
		planet1->SetParent(star1);
		planet1->AddComponent<Planet>(6456, 330.0f);
		planet1->AddComponent<Orbit>();
		planet1->AddComponent<Mesh>();*/

		//auto planet2{GetStructure()->CreateEntity({{0.0f, 0.0f, 10000.0f}})}; // One moon
		//planet2->SetName("Planet2");
		//planet2->SetParent(star1);
		//planet2->AddComponent<Planet>(1244, 560.0f);
		//planet2->AddComponent<Orbit>();
		//planet2->AddComponent<Mesh>();

		//auto fog2{GetStructure()->CreateEntity({})};
		//fog2->SetName("Planet2_Fog");
		//fog2->SetParent(planet2);
		//fog2->AddComponent<Mesh>();
		//fog2->AddComponent<MaterialFog>();
		//fog2->AddComponent<MeshRender>();

		/*auto planet3{GetStructure()->CreateEntity({{(0.0f, 0.0f, 18000.0f}})};
		planet3->SetName("Planet3");
		planet3->SetParent(star1);
		planet3->AddComponent<Planet>(1244, 800.0f);
		planet3->AddComponent<Orbit>();
		planet3->AddComponent<Mesh>();*/

		//auto planet4{GetStructure()->CreateEntity({{0.0f, 0.0f, 38000.0f}})}; // Two moons
		//planet4->SetName("Planet4");
		//planet4->SetParent(star1);
		//planet4->AddComponent<Planet>(-1, 2500.0f, 1330.0f);
		//planet4->AddComponent<Orbit>();
		//planet4->AddComponent<Mesh>();
		//planet4->AddComponent<MaterialGasGiant>(Image2d::Create("GasGiants/JupiterLookup.png"), -60.0f, 0.0003f);
		//planet4->AddComponent<MeshRender>();

		//auto rings4{GetStructure()->CreateEntity({})};
		//rings4->SetName("Planet4_Rings");
		//rings4->SetParent(planet4);
		//rings4->AddComponent<Mesh>();
		//rings4->AddComponent<MaterialRing>();
		//rings4->AddComponent<MeshRender>();

		/*auto planet5 = GetStructure()->CreateEntity(Transform(Vector3(0.0f, 0.0f, 62000.0f))); // Three moons
		planet5->SetName("Plane5");
		planet5->SetParent(star1);
		planet5->AddComponent<Planet>(-1, 1600.0f, 1330.0f);
		planet5->AddComponent<Orbit>();
		planet5->AddComponent<Mesh>();
		planet5->AddComponent<MaterialGasGiant>(Texture::Resource("GasGiants/SaturnLookup.png"), -20.0f, 0.0003f);
		planet5->AddComponent<MeshRender>();*/

		/*auto planet6 = GetStructure()->CreateEntity(Transform(Vector3(0.0f, 0.0f, 79000.0f))); // One moon
		planet6->SetName("Plane6");
		planet6->SetParent(star1);
		planet6->AddComponent<Planet>(-1, 1700.0f, 1330.0f);
		planet6->AddComponent<Orbit>();
		planet6->AddComponent<Mesh>();
		planet6->AddComponent<MaterialGasGiant>(Texture::Resource("GasGiants/SaturnLookup2.png"), 50.0f, 0.0003f);
		planet6->AddComponent<MeshRender>();*/
	}

	void Scene1::Update()
	{
		
	}

	bool Scene1::IsPaused() const
	{
		return !m_uiStartLogo.IsFinished();
	}
}
