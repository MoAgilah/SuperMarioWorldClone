#include "Player.h"

#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Resources/SFShader.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
//#include "../GameStates/PlayerState.h"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <format>
#include <iostream>

Player::Player(const Vector2f& pos)
	: DynamicGameObject(std::make_shared<SFAnimatedSprite>("Mario", 14, 4, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(9,16), Vector2f()))
		, m_airTimer(0.4f), m_invulTimer(1)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
	SetSpawnLoc();
	m_volume->Update(GetPosition());

	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });

	auto shader = dynamic_cast<SFShader*>(GameManager::Get()->GetShaderMgr().GetShader("Flash"));
	if (shader)
		m_fragShader = &shader->GetNativeShader();

	m_fragShader->setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));
	m_invulTimer.SetCurrTime(0);
}

void Player::Update(float deltaTime)
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;
	//PhysicsController* physCtrl = GetPhysicsController();
	GameManager* gameMgr = GameManager::Get();
	if (!gameMgr)
		return;

	ProcessInput();

	m_stateMgr.Update(deltaTime);
	spr->Update(deltaTime);
	//physCtrl->Update(GetDirection(), GetVelocity());

	if (GetIsAlive())
	{
		auto inputManager = gameMgr->GetInputManager();

		if (GetOnGround())
		{
			if (m_stateMgr.GetStateName() == "Airborne")
				m_stateMgr.PopState();

			if (GetIsCrouched())
			{
				/*if (m_stateMgr.GetStateName() != "Crouching")
					m_stateMgr.ChangeState(new CrouchingState(this));*/
			}
			else
			{
				/*if (m_stateMgr.GetStateName() != "Grounded")
					m_stateMgr.ChangeState(new GroundedState(this));*/
			}

			if (GetOnSlope())
			{
				/*if (physCtrl->GetPhysicsType() != PhysicsType::slope)
					physCtrl->SetOnSlope();*/

				if (!GetXVelocity())
				{
					if (GetShouldSlideLeft())
						SetSlideLeft(true);

					if (GetShouldSlideRight())
						SetSlideRight(true);
				}

				if (GetSlideLeft() || GetSlideRight())
				{
					if (inputManager->GetKeyState(Keys::LEFT_KEY) || inputManager->GetKeyState(Keys::RIGHT_KEY))
					{
						SetSlideLeft(false);
						SetSlideRight(false);
					}
					else
					{
						/*if (GetSlideLeft())
							DecrementXVelocity(physCtrl->GetXAcceleration());

						if (GetSlideRight())
							IncrementXVelocity(physCtrl->GetXAcceleration());*/

						if (spr->GetCurrentAnim() != MarioAnims::IDLE)
							spr->ChangeAnim(MarioAnims::IDLE);
					}
				}
			}
			else
			{
				/*if (physCtrl->GetPhysicsType() != PhysicsType::ground)
					physCtrl->SetWalking();*/

				SetSlideLeft(false);
				SetSlideRight(false);
				SetShouldSlideLeft(false);
				SetShouldSlideRight(false);
			}

			SetYVelocity(0);

			if (!GetXVelocity() && !GetYVelocity())
			{
				if (!inputManager->GetKeyState(Keys::DOWN_KEY) && !inputManager->GetKeyState(Keys::UP_KEY))
				{
					if (spr->GetCurrentAnim() != MarioAnims::IDLE)
						spr->ChangeAnim(MarioAnims::IDLE);
				}
			}
		}
		else
		{
			if (GetAirbourne())
			{
				/*if (m_stateMgr.GetStateName() != "Airborne")
					m_stateMgr.PushState(new AirborneState(this));*/

				m_airTimer.Update(deltaTime);
				if (m_airTimer.CheckEnd())
				{
					SetAirbourne(false);
					m_airTimer.RestartTimer();
				}
				else
				{
					/*if (m_airTimer.GetCurrTime() < physCtrl->GetAirTime() * 0.6f)
						DecrementYVelocity(physCtrl->GetYAcceleration());*/
				}
			}
			else
			{
				IncrementYVelocity(GameConstants::Gravity);
				if (!GetOnSlope())
				{
					/*if (physCtrl->GetPhysicsType() != PhysicsType::drop)*/
					{
						if (!GetIsCrouched())
						{
							if (spr->GetCurrentAnim() != MarioAnims::SPINJUMP)
								spr->ChangeAnim(FALL);
						}

						//physCtrl->SetFalling();
					}
				}
			}
		}

		if ((!inputManager->GetKeyState(Keys::LEFT_KEY) && !inputManager->GetKeyState(Keys::RIGHT_KEY)) && (!GetSlideLeft() && !GetSlideRight()))
			SetXVelocity(0.0f);

		//decomposition of movement
		if (GetXVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(Vector2f(GetXVelocity() * GameConstants::FPS * deltaTime, 0));
			gameMgr->GetCollisionMgr()->ProcessCollisions(this);
		}

		//CheckForHorizontalBounds(deltaTime);
		//SetGoalHit(GetPosition().x > GameConstants::RightMost);

		if (GetGoalHit())
		{
			SetSpawnLoc();

			//if (!GameConstants::Automated)
				Reset();
		}

		if (GetYVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(Vector2f(0, GetYVelocity() * GameConstants::FPS * deltaTime));
			gameMgr->GetCollisionMgr()->ProcessCollisions(this);
		}


		/*if (gameMgr->GetCamera().CheckVerticalBounds(GetAABB()))
			SetIsAlive(false, 0.4f);*/

		if (gameMgr->GetTimer().CheckEnd())
			SetIsAlive(false);

		if (GetIfInvulnerable())
		{
			m_invulTimer.Update(deltaTime);
			m_fragShader->setUniform("time", m_invulTimer.GetCurrTime());
		}
	}
	else
	{
		/*if (m_stateMgr.GetStateName() != "Dieing")
			m_stateMgr.ChangeState(new DieingState(this));*/
	}
}

void Player::Render(IRenderer* renderer)
{
	if (!renderer || !m_drawable || !m_fragShader)
		return;

	//window.draw(*GetSprite()->GetSprite(), m_fragShader);
#if defined _DEBUG
	m_volume->Render(renderer);
#endif
}

void Player::Reset()
{
	SetInitialPosition(m_spawnLoc);
	DynamicGameObject::Reset();
	SetIsSuper(false);

	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != MarioAnims::IDLE)
			spr->ChangeAnim(MarioAnims::IDLE);
	}

	m_crouched = false;
	m_alive = true;
	m_cantjump = false;
	m_cantSpinJump = false;
	m_goalHit = false;

	m_invulTimer.SetCurrTime(0);
	m_airTimer.RestartTimer();
	m_stateMgr.ClearStates();
	GameManager::Get()->GetTimer().RestartTimer();
	GameManager::Get()->GetScene()->ResetScene();
}

void Player::SetIsSuper(bool super)
{
	m_super = super;

	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;

	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
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

void Player::ProcessInput()
{
	if (!GetIsAlive())
		return;

	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;

	Input();

	m_stateMgr.ProcessInputs();

	if (GameManager::Get()->GetInputManager()->GetKeyState(Keys::DOWN_KEY))
	{
		if (GetOnSlope())
		{
			if (spr->GetCurrentAnim() != MarioAnims::SLIDE)
				spr->ChangeAnim(MarioAnims::SLIDE);
		}
		else
		{
			if (!GetIsCrouched())
				SetIsCrouched(true);
		}
	}
	else
	{
		if (GetOnSlope())
		{
			if (spr->GetCurrentAnim() == MarioAnims::SLIDE)
				spr->ChangeAnim(MarioAnims::IDLE);
		}
		else
		{
			if (GetIsCrouched())
				SetIsCrouched(false);
		}
	}
}

void Player::Input()
{
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		inputManager.GetKeyState(Keys::LEFT_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys::LEFT_KEY))
			inputManager.GetKeyState(Keys::LEFT_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		inputManager.GetKeyState(Keys:RIGHT_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys:RIGHT_KEY))
			inputManager.GetKeyState(Keys:RIGHT_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		inputManager.GetKeyState(Keys::UP_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys::UP_KEY))
			inputManager.GetKeyState(Keys::UP_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		inputManager.GetKeyState(Keys::DOWN_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys::DOWN_KEY))
			inputManager.GetKeyState(Keys::DOWN_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_keyStates[Keys::JUMP_KEY] = true;
	}
	else
	{
		if (m_keyStates[Keys::JUMP_KEY])
			m_keyStates[Keys::JUMP_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_keyStates[Keys::RUN_KEY] = true;
	}
	else
	{
		if (m_keyStates[Keys::RUN_KEY])
			m_keyStates[Keys::RUN_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_keyStates[Keys::SJUMP_KEY] = true;
	}
	else
	{
		if (m_keyStates[Keys::SJUMP_KEY])
			m_keyStates[Keys::SJUMP_KEY] = false;
	}*/
}