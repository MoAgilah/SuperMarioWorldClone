#include "Player.h"

#include "../Engine/States/PlayerState.h"
#include "../Utilities/GameMode.h"
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Resources/SFShader.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <format>
#include <iostream>

Player::Player(const Vector2f& pos)
	: DynamicGameObject(std::make_shared<SFAnimatedSprite>("Mario", 14, 4, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(9,16)))
	, m_airTimer(0.6f), m_invulTimer(1)
{
	m_dynType = typeid(*this);

	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
	SetSpawnLoc();

	ENSURE_VALID(m_volume);
	m_volume->Update(GetPosition());

	DECL_GET_OR_RETURN(spr, GetAnimatedSprite());

	spr->SetFrames({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });

	auto shader = GameManager::Get()->GetShaderMgr().GetShader("FlashShader");
	if (shader)
	{
		m_fragShader = shader;
		GetShader()->GetNativeShader().setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));
	}

	m_invulTimer.SetCurrTime(0);

	m_stateMgr.ChangeState(new LateralState(this));
}

void Player::Update(float deltaTime)
{
	ENSURE_VALID(m_drawable);
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

	PlayerState::s_frameStep = GameConstants::FPS * deltaTime;

	ProcessInput();

	m_stateMgr.Update(deltaTime);
	m_drawable->Update(deltaTime);

	auto pos = m_drawable->GetPosition();

	if (GetIsAlive())
	{
		DECL_GET_OR_RETURN(inputManager, gameMgr->GetInputManager());

		if (GetOnGround())
		{
			if (m_stateMgr.GetStateName() == "Airborne")
				m_stateMgr.PopState();
			SetYVelocity(0);
		}
		else
		{
			if (m_stateMgr.GetStateName() != "Airborne")
			{
				SetAirbourne(true);
				m_stateMgr.PushState(new VerticalState(this, false, true));
			}
		}

		if ((!inputManager->GetKeyState(Keys::LEFT_KEY) && !inputManager->GetKeyState(Keys::RIGHT_KEY)))
			SetXVelocity(0.0f);

		//decomposition of movement
		if (GetXVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(Vector2f(GetXVelocity() * PlayerState::s_frameStep, 0));
			colMgr->ProcessCollisions(this);
		}

		GameMode::CheckForHorizontalBounds(deltaTime, this);

		if (GetYVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(Vector2f(0, GetYVelocity() * PlayerState::s_frameStep));
			colMgr->ProcessCollisions(this);
		}

		if (GetIfInvulnerable())
		{
			m_invulTimer.Update(deltaTime);
			GetShader()->GetNativeShader().setUniform("time", m_invulTimer.GetCurrTime());
		}
	}
	else
	{
		if (m_stateMgr.GetStateName() != "Dieing")
			m_stateMgr.ChangeState(new DieingState(this));
	}
}

void Player::Render(IRenderer* renderer)
{
	ENSURE_VALID(renderer);
	ENSURE_VALID(m_drawable);
	ENSURE_VALID(m_fragShader);

	m_drawable->Render(renderer, m_fragShader);
#if defined _DEBUG

	ENSURE_VALID(m_volume);
	m_volume->Render(renderer);
#endif
}

void Player::OnCollisionEnter(IGameObject* obj)
{
}

void Player::OnCollisionStay(IGameObject* obj)
{

}

void Player::OnCollisionExit(IGameObject* obj)
{

}

void Player::ResolveCollisions(float time, const Vector2f& separationVector, float relativeHitPosition)
{

}

void Player::Reset()
{
	SetInitialPosition(m_spawnLoc);
	DynamicGameObject::Reset();
	SetIsSuper(false);

	m_crouched = false;
	m_alive = true;
	m_cantjump = false;
	m_cantSpinJump = false;

	m_invulTimer.SetCurrTime(0);
	m_airTimer.RestartTimer();
	m_stateMgr.ClearStates();
	m_stateMgr.ChangeState(new LateralState(this));

	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());


	gameMgr->GetTimer().RestartTimer();

	DECL_GET_OR_RETURN(scene, gameMgr->GetScene());

	scene->ResetScene();
}

void Player::SetDirection(bool dir)
{
	IGameObject::SetDirection(dir);

	if (auto spr = dynamic_cast<ISprite*>(m_drawable.get()))
		spr->SetDirection(dir);
}

void Player::SetIsSuper(bool super)
{
	m_super = super;

	DECL_GET_OR_RETURN(spr, GetAnimatedSprite());

	DECL_GET_OR_RETURN(box, GetBox());

	if (m_super)
	{
		if (spr->GetTexID() != "Super")
		{
			THROW_IF_FALSE_MSG(spr->SetTexture("Super"), "Failed to change the Player texId to {}", "Super");

			spr->SetFrameSize(Vector2u(spr->GetTextureSize().x / 4, spr->GetTextureSize().y / 14));

			box->Reset(m_boxSizes[MarioBoxes::SUPER]);

			Move(0, -m_heightDiff);
		}
	}
	else
	{
		if (spr->GetTexID() != "Mario")
		{
			THROW_IF_FALSE_MSG(spr->SetTexture("Mario"), "Failed to change the Player texId to {}", "Mario");

			spr->SetFrameSize(Vector2u(spr->GetTextureSize().x / 4, spr->GetTextureSize().y / 14));

			box->Reset(m_boxSizes[MarioBoxes::REGULAR]);

			Move(0, m_heightDiff);
		}
	}
}

void Player::SetIsAlive(bool val, float airtime)
{
	m_alive = val;
	if (!m_alive)
		m_airTimer.SetCurrTime(airtime);
}

void Player::SetSpawnLoc(Vector2f loc)
{
	if (loc == Vector2f(0, 0))
	{
		m_spawnLoc = GetInitialPosition();
	}
	else
	{
		m_spawnLoc.x = loc.x;
	}
}

void Player::SetIsCrouched(bool crouched)
{
	DECL_GET_OR_RETURN(box, GetBox());

	m_crouched = crouched;
	if (m_crouched)
	{
		box->Reset(m_boxSizes[MarioBoxes::CROUCHED]);
		if (m_super)
		{
			box->Update(box->GetPosition() + Vector2f(0, 15));
		}
		else
		{
			box->Update(box->GetPosition() + Vector2f(0, 5));
		}
	}
	else
	{
		if (m_super)
		{
			box->Reset(m_boxSizes[MarioBoxes::SUPER]);
			box->Update(box->GetPosition() - Vector2f(0, 15));
		}
		else
		{
			box->Reset(m_boxSizes[MarioBoxes::REGULAR]);
			box->Update(box->GetPosition() - Vector2f(0, 5));
		}
	}
}

void Player::ForceFall()
{
	SetCantJump(true);
	SetCantSpinJump(true);
	SetAirbourne(false);
}

void Player::Bounce()
{
	SetYVelocity(-(GetYVelocity() / 2));
	SetXVelocity(GetDirection() ? GetXVelocity() : -GetXVelocity());
}

void Player::SetInvulnerability()
{
	m_invulTimer.RestartTimer();
}

void Player::SetAirbourne(bool air)
{
	m_airbourne = air;
	if (m_airbourne)
		SetOnGround(false);
}

SFAnimatedSprite* Player::GetAnimatedSprite()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		return spr;

	return nullptr;
}

SFShader* Player::GetShader()
{
	auto shader = dynamic_cast<SFShader*>(m_fragShader);
	if (shader)
		return shader;

	return nullptr;
}

BoundingBox<SFRect>* Player::GetBox()
{
	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
	if (box)
		return box;

	return nullptr;
}

void Player::ProcessInput()
{
	ENSURE_VALID(m_drawable);

	if (!GetIsAlive())
		return;

	Input();
}

void Player::Input()
{
	m_stateMgr.ProcessInputs();
}