#include "Player.h"

#include "../Engine/States/PlayerState.h"
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Resources/SFShader.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <format>
#include <iostream>

Player::Player(const Vector2f& pos)
	: DynamicGameObject(std::make_shared<SFAnimatedSprite>("Mario", 14, 4, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(9,16)))
	, m_airTimer(0.4f), m_invulTimer(1)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
	SetSpawnLoc();
	m_volume->Update(GetPosition());

	auto spr = GetAnimatedSprite();
	if (spr)
		spr->SetFrames({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });

	auto shader = GameManager::Get()->GetShaderMgr().GetShader("FlashShader");
	if (shader)
	{
		m_fragShader = shader;
		GetShader()->GetNativeShader().setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));
	}

	m_invulTimer.SetCurrTime(0);
}

void Player::Update(float deltaTime)
{
	auto gameMgr = GameManager::Get();

	if (!m_drawable && !gameMgr)
		return;

	ProcessInput();

	m_stateMgr.Update(deltaTime);
	m_drawable->Update(deltaTime);

	ProcessInput();

	if (GetIsAlive())
	{
		auto inputManager = gameMgr->GetInputManager();

		if (GetOnGround())
			SetYVelocity(0);

		if ((!inputManager->GetKeyState(Keys::LEFT_KEY) && !inputManager->GetKeyState(Keys::RIGHT_KEY)))
			SetXVelocity(0.0f);

		//decomposition of movement
		if (GetXVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(Vector2f(GetXVelocity() * GameConstants::FPS * deltaTime, 0));
			gameMgr->GetCollisionMgr()->ProcessCollisions(this);
		}

		if (GetYVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(Vector2f(0, GetYVelocity() * GameConstants::FPS * deltaTime));
			gameMgr->GetCollisionMgr()->ProcessCollisions(this);
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
	if (!renderer || !m_drawable || !m_fragShader)
		return;

	m_drawable->Render(renderer, m_fragShader);
#if defined _DEBUG
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
	GameManager::Get()->GetTimer().RestartTimer();
	GameManager::Get()->GetScene()->ResetScene();
}

void Player::SetIsSuper(bool super)
{
	m_super = super;

	auto spr = GetAnimatedSprite();
	if (!spr)
		return;

	auto box = GetBox();
	if (!box)
		return;

	if (m_super)
	{
		if (spr->GetTexID() != "Super")
		{
			spr->SetTexture("Super");
			spr->SetFrameSize(Vector2u(spr->GetTextureSize().x / 4, spr->GetTextureSize().y / 14));

			box->Reset(m_boxSizes[MarioBoxes::SUPER]);

			Move(0, -m_heightDiff);
		}
	}
	else
	{
		if (spr->GetTexID() != "Mario")
		{
			spr->SetTexture("Mario");
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
	{
		m_airTimer.SetCurrTime(airtime);
	}
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
	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
	if (!box)
		return;

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
	if (!m_drawable && !GetIsAlive())
		return;

	Input();
}

void Player::Input()
{
	m_stateMgr.ProcessInputs();
}