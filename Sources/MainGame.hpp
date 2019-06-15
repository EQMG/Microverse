#pragma once

#include <Engine/Game.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include "Configs/ConfigManager.hpp"

using namespace acid;

namespace micro
{
class MainGame :
	public Game,
	public Observer
{
public:
	MainGame();

	~MainGame();

	void Update() override;

private:
	std::unique_ptr<ConfigManager> m_configs;

	ButtonKeyboard m_buttonFullscreen;
	ButtonKeyboard m_buttonScreenshot;
	ButtonKeyboard m_buttonExit;
};
}
