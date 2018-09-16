#pragma once

#include <Audio/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Inputs/ButtonMouse.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Scenes/IScene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"
#include "Uis/Navigation/UiNavigation.hpp"

using namespace acid;

namespace micro
{
	class Scene1 :
		public IScene
	{
	private:
		ButtonMouse m_buttonSpawnSphere;
		ButtonKeyboard m_buttonFullscreen;
		ButtonKeyboard m_buttonCaptureMouse;
		ButtonKeyboard m_buttonScreenshot;
		ButtonKeyboard m_buttonPause;
		ButtonKeyboard m_buttonExit;
		Sound m_soundScreenshot;

		std::unique_ptr<UiStartLogo> m_uiStartLogo;
		std::unique_ptr<OverlayDebug> m_overlayDebug;
		std::unique_ptr<UiNavigation> m_uiNavigation;
	public:
		Scene1();

		void Start() override;

		void Update() override;

		bool IsPaused() const override;
	private:
		void TogglePause();
	};
}
