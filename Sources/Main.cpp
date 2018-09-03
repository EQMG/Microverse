#include <iostream>
#include <Files/Files.hpp>
#include <Files/Json/FileJson.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "Celestial/Fogs/MaterialFog.hpp"
#include "Celestial/GasGiants/MaterialGasGiant.hpp"
#include "Celestial/Rings/MaterialRing.hpp"
#include "Celestial/Gravity.hpp"
#include "Celestial/Planet.hpp"
#include "Celestial/Star.hpp"
#include "Chunks/QuadtreeChunk.hpp"
#include "Chunks/MaterialChunk.hpp"
#include "World/World.hpp"
#include "Configs/ConfigManager.hpp"
#include "MainRenderer.hpp"
#include "Scenes/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"

using namespace micro;
using namespace acid;

int main(int argc, char **argv)
{
	// Registers file search paths.
	Files::AddSearchPath("Resources/Game");
	Files::AddSearchPath("Resources/Engine");

	// Creates the engine.
	auto engine = std::make_unique<Engine>();

	auto configManager = std::make_unique<ConfigManager>();
	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	// Registers modules.
	Engine::Get()->RegisterModule<World>(UPDATE_NORMAL);

	// Registers components.
	Scenes::Get()->RegisterComponent<FpsPlayer>("FpsPlayer");
	Scenes::Get()->RegisterComponent<Gravity>("Gravity");
	Scenes::Get()->RegisterComponent<Planet>("Planet");
	Scenes::Get()->RegisterComponent<Star>("Star");
	Scenes::Get()->RegisterComponent<MaterialFog>("MaterialFog");
	Scenes::Get()->RegisterComponent<MaterialGasGiant>("MaterialGasGiant");
	Scenes::Get()->RegisterComponent<MaterialRing>("MaterialRing");
	Scenes::Get()->RegisterComponent<QuadtreeChunk>("QuadtreeChunk");
	Scenes::Get()->RegisterComponent<MaterialChunk>("MaterialChunk");

	// Initializes modules.
	Display::Get()->SetTitle("Microverse");
	Display::Get()->SetIcon("Logos/Microverse.png");
	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
//	std::cin.get();
	return exitCode;
}
