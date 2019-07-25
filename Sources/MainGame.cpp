#include "MainGame.hpp"

#include <iostream>
#include <Files/Files.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Timers/Timers.hpp>
//#include "Celestial/Fogs/MaterialFog.hpp"
//#include "Celestial/GasGiants/MaterialGasGiant.hpp"
//#include "Celestial/Rings/MaterialRing.hpp"
#include "Celestial/Gravity.hpp"
#include "Celestial/Planet.hpp"
#include "Celestial/Star.hpp"
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"
#include "World/World.hpp"
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"
#include "World/World.hpp"
#include "Resources/Resources.hpp"

int main(int argc, char **argv)
{
	using namespace micro;

	// Creates the engine.
	auto engine{std::make_unique<Engine>(argv[0])};
	engine->SetGameName("Test Physics");
	engine->SetGame(std::make_unique<MainGame>());

	// Runs the game loop.
	auto exitCode{engine->Run()};

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}

namespace micro
{
MainGame::MainGame() :
	m_buttonFullscreen{Key::F11},
	m_buttonScreenshot{Key::F9},
	m_buttonExit{Key::Delete}
{
	// Registers file search paths.
	Log::Out("Working Directory: %ls\n", std::filesystem::current_path());
	Files::Get()->AddSearchPath("Resources/Engine");
	Files::Get()->AddSearchPath("Resources/Game");

	// Loads configs from a config manager.
	m_configs = std::make_unique<ConfigManager>();

	m_buttonFullscreen.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}
	});
	m_buttonScreenshot.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Resources::Get()->GetThreadPool().Enqueue([]()
			{
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	});
	m_buttonExit.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Engine::Get()->RequestClose();
		}
	});

	// Registers modules.
	Engine::Get()->AddModule<World>(Module::Stage::Always);
	//Engine::Get()->RemoveModule<Shadows>();

	// Registers components.
	auto &componentRegister{Scenes::Get()->GetComponentRegister()};
	componentRegister.Add<Gravity>("Gravity");
	componentRegister.Add<Planet>("Planet");
	componentRegister.Add<Star>("Star");
	//componentRegister.Add<MaterialFog>("MaterialFog");
	//componentRegister.Add<MaterialGasGiant>("MaterialGasGiant");
	//componentRegister.Add<MaterialRing>("MaterialRing");
	componentRegister.Add<QuadtreeChunk>("QuadtreeChunk");
	componentRegister.Add<MaterialChunk>("MaterialChunk");

	// Sets values to modules.
	Window::Get()->SetTitle("Test Physics");
	Window::Get()->SetIcons({"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png", "Icons/Icon-96.png",
		"Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"}); // Logos/Microverse.png
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
}

MainGame::~MainGame()
{
	m_configs->Save();
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

void MainGame::Update()
{
}
}
