#include "FpsPlayer.hpp"

#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>
#include <World/World.hpp>

namespace test
{
	static const float WALK_SPEED = 10.0f; // 3.1f
	static const float RUN_SPEED = 25.0f; // 5.7f
	static const float CROUCH_SPEED = 1.2f;
	static const float JUMP_SPEED = 6.5f;
	static const float CROUCH_JUMP_SPEED = 3.0f;
	static const float NOCLIP_SPEED = 30.0f;
	static const float GRAVITY = -9.81f;
	static const Vector3 DAMP_NOCLIP = Vector3(10.0f, 10.0f, 10.0f);
	static const Vector3 DAMP_NORMAL = Vector3(20.0f, 1.0f, 20.0f);

	FpsPlayer::FpsPlayer() :
		IBehaviour(),
		m_velocity(new Vector3()),
		m_jumping(false),
		m_noclipEnabled(true),
		m_inputForward(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({KEY_S, KEY_DOWN}),
				new ButtonKeyboard({KEY_W, KEY_UP})
			),
			new AxisJoystick(JOYSTICK_1, {1}, true)
		})),
		m_inputStrafe(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({KEY_D, KEY_RIGHT}),
				new ButtonKeyboard({KEY_A, KEY_LEFT})
			),
			new AxisJoystick(JOYSTICK_1, {0}, true)
		})),
		m_inputSprint(new ButtonCompound({
			new ButtonKeyboard({KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT}),
			new ButtonJoystick(JOYSTICK_1, {1})
		})),
		m_inputJump(new ButtonCompound({
			new ButtonKeyboard({KEY_SPACE}),
			new ButtonJoystick(JOYSTICK_1, {1})
		})),
		m_inputCrouch(new ButtonCompound({
			new ButtonKeyboard({KEY_LEFT_CONTROL, KEY_RIGHT_CONTROL}),
			new ButtonJoystick(JOYSTICK_1, {1})
		})),
		m_toggleNoclip(new ButtonCompound({
			new ButtonKeyboard({KEY_N}),
		})),
		m_amountMove(new Vector3()),
		m_amountRotate(new Vector3())
	{
	}

	FpsPlayer::~FpsPlayer()
	{
		delete m_velocity;

		delete m_inputForward;
		delete m_inputStrafe;

		delete m_inputSprint;
		delete m_inputCrouch;
		delete m_inputJump;
		delete m_toggleNoclip;

		delete m_amountMove;
		delete m_amountRotate;
	}

	void FpsPlayer::Update()
	{
		// Gets the delta and limits the lowest UPS to 20 (any less and the game is unplayable).
		float delta = std::min(Engine::Get()->GetDelta(), 1.0f / 20.0f);

		Vector3 targetVelocity = Vector3(0.0f, m_noclipEnabled ? 0.0f : GRAVITY, 0.0f);

		/*if (!m_noclipEnabled)
		{
			Vector3 cartesian = GetGameObject()->GetTransform()->GetPosition() - Vector3::ZERO;
			Vector3 polar = cartesian.CartesianToPolar();
			polar.m_x = GRAVITY;
			targetVelocity = polar.PolarToCartesian();
		}*/

		if (!Scenes::Get()->IsGamePaused())
		{
			bool sprintDown = m_inputSprint->IsDown();
			bool crouchDown = m_inputCrouch->IsDown();

			targetVelocity.m_z += (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputForward->GetAmount();
			targetVelocity.m_x += (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputStrafe->GetAmount();

			if (m_noclipEnabled)
			{
				if (m_inputJump->IsDown())
				{
					targetVelocity.m_y += sprintDown ? RUN_SPEED : WALK_SPEED;
				}
				else if (m_inputCrouch->IsDown())
				{
					targetVelocity.m_y += sprintDown ? -RUN_SPEED : -WALK_SPEED;
				}

				targetVelocity *= NOCLIP_SPEED;
			}
			else
			{
				if (m_inputJump->WasDown() && !m_jumping)
				{
					targetVelocity.m_y += crouchDown ? CROUCH_JUMP_SPEED : JUMP_SPEED;
					m_velocity->m_y += targetVelocity.m_y;
					m_jumping = true;
				}
			}

			if (m_toggleNoclip->WasDown())
			{
				targetVelocity *= 0.0f;
				m_jumping = false;
				m_noclipEnabled = !m_noclipEnabled;
				printf("Player Noclip: %s\n", m_noclipEnabled ? "true" : "false");
			}
		}

		*m_velocity = m_velocity->SmoothDamp(targetVelocity, delta * (m_noclipEnabled ? DAMP_NOCLIP : DAMP_NORMAL));

		auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
		Vector3 position = GetGameObject()->GetTransform().GetPosition();
		Vector3 rotation = GetGameObject()->GetTransform().GetRotation();

		// Planet collision.
		/*if (!m_noclipEnabled)
		{
			Vector3 cartesian = position - Vector3::ZERO;
			Vector3 polar = cartesian.CartesianToPolar();
			float planetRadius = World::Get()->GetTerrainRadius(700.0f, polar.m_y, polar.m_z) + 1.74f;
			polar.m_x = std::max(polar.m_x, planetRadius);
			cartesian = polar.PolarToCartesian();
			position = cartesian;

			if (polar.m_x <= planetRadius)
			{
			//	m_velocity->m_y = 0.0f;
				m_jumping = false;
			//	newPosition.m_y = groundHeight;
			}
		}*/

		// Calculates the deltas to the moved distance, and rotation.
		float theta = Maths::Radians(cameraRotation.m_y);
		float dx = -(m_velocity->m_z * std::sin(theta) + m_velocity->m_x * std::cos(theta)) * delta;
		float dy = m_velocity->m_y * delta;
		float dz = -(m_velocity->m_z * std::cos(theta) - m_velocity->m_x * std::sin(theta)) * delta;

		*m_amountMove = Vector3(dx, dy, dz);
		*m_amountRotate = Vector3(0.0f, 0.0f, 0.0f);

		position += *m_amountMove;
		rotation += *m_amountRotate;

		GetGameObject()->GetTransform().SetPosition(position);
		GetGameObject()->GetTransform().SetRotation(rotation);

		// GetGameObject()->GetTransform().SetPosition(Vector3(0.0f, 1000.0f * std::cos(Engine::Get()->GetTime() / 10.0f), 4000.0f * std::sin(Engine::Get()->GetTime() / 8.0f)));
	}
}
