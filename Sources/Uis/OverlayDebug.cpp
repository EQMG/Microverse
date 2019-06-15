﻿#include "OverlayDebug.hpp"

#include <Scenes/Scenes.hpp>

namespace micro
{
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject{parent, {{100, 36}, UiAnchor::LeftBottom}},
	m_textFrameTime{this, {{100, 12}, UiAnchor::LeftBottom, {2, -2}}, 11, "", FontType::Create("Fonts/ProximaNova")},
	m_textFps{this, {{100, 12}, UiAnchor::LeftBottom, {2, -16}}, 11, "", FontType::Create("Fonts/ProximaNova")},
	m_textUps{this, {{100, 12}, UiAnchor::LeftBottom, {2, -30}}, 11, "", FontType::Create("Fonts/ProximaNova")}
{
}

void OverlayDebug::UpdateObject()
{
	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
