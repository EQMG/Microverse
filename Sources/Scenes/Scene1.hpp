#pragma once

#include <Audio/Sound.hpp>
#include <Inputs/IButton.hpp>
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
		IButton *m_buttonSpawnSphere;
		IButton *m_buttonFullscreen;
		IButton *m_buttonCaptureMouse;
		IButton *m_buttonScreenshot;
		IButton *m_buttonExit;
		Sound *m_soundScreenshot;

		Colour m_primaryColour;
		SelectorJoystick *m_selectorJoystick;

		IButton *m_buttonPause;

		UiStartLogo *m_uiStartLogo;
		OverlayDebug *m_overlayDebug;
		UiNavigation *m_uiNavigation;
	public:
		Scene1();

		~Scene1();

		void Start() override;

		void Update() override;

		bool IsGamePaused() const override;

		Colour GetUiColour() const override { return m_primaryColour; }

		SelectorJoystick *GetSelectorJoystick() const override { return m_selectorJoystick; };
	private:
		void TogglePause();
	};
}
