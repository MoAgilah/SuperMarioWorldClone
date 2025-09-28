#include "PlayerState.h"

#include "../../GameObjects/Player.h"
#include <Drawables/SFSprite.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <format>
#include <iostream>

MovementController PlayerState::m_movementCtrl = {};

PlayerState::PlayerState(Player* ply)
	: IObjectState(ply)
{
	if (!m_initialisedCtrl)
	{
		Physics grounded;

		grounded.AddVelocity("Walk", 1.5f);
		grounded.AddVelocity("Run", 2.5f);
		grounded.AddVelocity("Sprint", 3.5f);
		grounded.AddVelocity("EOL", 1.25f);

		grounded.AddAcceleration("Imp", 0.0546875f);

		grounded.AddDeceleration("Stop", 0.0546875f);
		grounded.AddDeceleration("Skid", 0.046875f);

		m_movementCtrl.AddPhysicsState("Grounded", grounded);

		Physics sloped;

		sloped.AddVelocity("Walking", 0.8125f);
		sloped.AddVelocity("Running", 1.375f);
		sloped.AddVelocity("Sliding", 3.9375f);

		grounded.AddAcceleration("G26", 0.125f);
		grounded.AddAcceleration("G45", 0.1875f);
		grounded.AddAcceleration("GS26", 0.0625f);
		grounded.AddAcceleration("GS45", 0.125f);

		m_movementCtrl.AddPhysicsState("Sloped", sloped);

		Physics jumping;

		jumping.AddVelocity("Low", 3.4375f);		// X Velocity < 1 px/f (-Y)
		jumping.AddVelocity("Med", 3.5625f);		// X Velocity > 1 px/f (-Y)
		jumping.AddVelocity("High", 3.6875f);		// X Velocity > 2 px/f (-Y)
		jumping.AddVelocity("Max", 3.9375f);		// X Velocity > 3 px/f (-Y)

		jumping.AddVelocity("Slam", 4.f);			// -Y
		jumping.AddVelocity("Tap", 3.f);			// -Y

		jumping.AddAcceleration("High", 0.0625f);	//(A held, Y Vel < -2 px/f)
		jumping.AddAcceleration("Low", 0.3125f);	//(Y Vel > -2 px / f)

		m_movementCtrl.AddPhysicsState("Jumping", jumping);

		m_initialisedCtrl = true;
	}
}

PlayerState::~PlayerState()
{
	if (m_gameObj)
		m_gameObj = nullptr;
}

void PlayerState::Pause()
{
	GetPlayer()->SetActive(false);
}

void PlayerState::Resume()
{
	GetPlayer()->SetActive(true);
}

SFAnimatedSprite* PlayerState::GetAnimSpr()
{
	return nullptr;
}

Player* PlayerState::GetPlayer()
{
	auto ply = dynamic_cast<Player*>(m_gameObj);
	if (ply)
		return ply;

	return nullptr;
}

GroundedState::GroundedState(Player* ply)
	: PlayerState(ply)
{
}

void GroundedState::Initialise()
{
	auto animSpr = GetAnimSpr();
	if (animSpr)
	{
		if (animSpr->GetCurrentAnim() != MarioAnims::IDLE)
			animSpr->ChangeAnim(MarioAnims::IDLE);
	}
}

void GroundedState::Resume()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();

	if (ply && animSpr)
	{
		if (ply->GetXVelocity() == 0)
			animSpr->ChangeAnim(MarioAnims::IDLE);
		else
			UpdateAnimation();
	}

	PlayerState::Resume();
}

void GroundedState::ProcessInputs()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();
	auto inputManager = GameManager::Get()->GetInputManager();

	if (!ply || !animSpr || !inputManager)
		return;

	if (inputManager->GetKeyState(Keys::LEFT_KEY) && inputManager->GetKeyState(Keys::RIGHT_KEY))
	{
		// prioritize multi keys based on previous direction
		if (ply->GetDirection())
		{
			UpdateAnimation();
			//ply->IncrementXVelocity(m_physCtrl->GetXAcceleration());
		}
		else
		{
			UpdateAnimation();
			//ply->DecrementXVelocity(m_physCtrl->GetXAcceleration());
		}
	}
	else
	{
		if (inputManager->GetKeyState(Keys::LEFT_KEY))
		{
			if (ply->GetDirection())
			{
				ply->SetDirection(false);
				/*if (ply->GetXVelocity() > m_physCtrl->GetXAcceleration())
					Slide(ply->GetDirection());*/
			}

			/*if (!m_turningAround)
			{
				UpdateAnimation();
				ply->DecrementXVelocity(m_physCtrl->GetXAcceleration());
			}*/
		}

		if (inputManager->GetKeyState(Keys::RIGHT_KEY))
		{
			if (!ply->GetDirection())
			{
				ply->SetDirection(true);
				/*if (ply->GetXVelocity() < -m_physCtrl->GetXAcceleration())
					Slide(ply->GetDirection());*/
			}

			/*if (!m_turningAround)
			{
				UpdateAnimation();
				ply->IncrementXVelocity(m_physCtrl->GetXAcceleration());
			}*/
		}
	}

	if (inputManager->GetKeyState(Keys::UP_KEY))
	{
		if (!inputManager->GetKeyState(Keys::LEFT_KEY) && !inputManager->GetKeyState(Keys::RIGHT_KEY))
		{
			if (animSpr->GetCurrentAnim() != MarioAnims::LOOKUP)
				animSpr->ChangeAnim(MarioAnims::LOOKUP);
		}
	}

	if (inputManager->GetKeyState(Keys::RUN_KEY))
	{
		/*if (m_physCtrl->GetXVelocityType() == XVelocity::walking)
			m_physCtrl->SetRunning();*/
	}
	else
	{
		/*if (m_physCtrl->GetXVelocityType() != XVelocity::walking)
			m_physCtrl->SetWalking();*/
	}

	if (inputManager->GetKeyState(Keys::JUMP_KEY))
	{
		if (ply->GetOnGround() && !ply->GetCantJump())
		{
			animSpr->UpdateAnimSpeed(0.5f);
			/*switch (m_physCtrl->GetXVelocityType())
			{
			case XVelocity::walking:
				[[fallthrough]];
			case XVelocity::running:
				if (animSpr->GetCurrentAnim() != MarioAnims::JUMP)
					animSpr->ChangeAnim(MarioAnims::JUMP);
				break;
			case XVelocity::sprinting:
				if (animSpr->GetCurrentAnim() != MarioAnims::RUNJUMP)
					animSpr->ChangeAnim(MarioAnims::RUNJUMP);
				break;
			}*/

			ply->SetAirbourne(true);
			ply->SetCantJump(true);
			/*if (m_physCtrl->GetPhysicsType() != PhysicsType::rise)
			{
				m_physCtrl->SetAerial();
				ply->GetAirTimer()->SetTime(m_physCtrl->GetAirTime());
			}*/
		}
	}
	else
	{
		ply->SetCantJump(false);
	}

	if (inputManager->GetKeyState(Keys::SJUMP_KEY))
	{
		if (ply->GetOnGround() && !ply->GetCantSpinJump())
		{
			animSpr->UpdateAnimSpeed(0.5f);
			if (animSpr->GetCurrentAnim() != MarioAnims::SPINJUMP)
				animSpr->ChangeAnim(MarioAnims::SPINJUMP);
			ply->SetAirbourne(true);
			ply->SetCantSpinJump(true);
			/*if (m_physCtrl->GetPhysicsType() != PhysicsType::rise)
			{
				m_physCtrl->SetAerial();
				ply->GetAirTimer()->SetTime(m_physCtrl->GetAirTime());
			}*/
		}
	}
	else
	{
		ply->SetCantSpinJump(false);
	}
}

void GroundedState::Update(float deltaTime)
{
	/*if (m_turningAround)
	{
		m_turnTimer.Update(deltaTime);
		if (m_turnTimer.CheckEnd())
			m_turningAround = false;
	}*/
}

void GroundedState::UpdateAnimation()
{
	auto animSpr = GetAnimSpr();

	if (!animSpr)
		return;

	/*switch (m_physCtrl->GetXVelocityType())
	{
	case XVelocity::walking:
		animSpr->UpdateAnimSpeed(0.5f);
		if (animSpr->GetCurrentAnim() != MarioAnims::MOVING)
			animSpr->ChangeAnim(MarioAnims::MOVING);
		break;
	case XVelocity::running:
		animSpr->UpdateAnimSpeed(0.75f);
		if (animSpr->GetCurrentAnim() != MarioAnims::MOVING)
			animSpr->ChangeAnim(MarioAnims::MOVING);
		break;
	case XVelocity::sprinting:
		animSpr->UpdateAnimSpeed(0.5f);
		if (animSpr->GetCurrentAnim() != MarioAnims::RUNNING)
			animSpr->ChangeAnim(MarioAnims::RUNNING);
		break;
	}*/
}

SlopedState::SlopedState(Player* ply)
	: PlayerState(ply)
{
}

void SlopedState::Initialise()
{
	auto player = GetPlayer();
	auto animSpr = player->GetAnimatedSprite();

	player->SetXVelocity(0);
	if (animSpr)
	{
		if (animSpr->GetCurrentAnim() != MarioAnims::CROUCH)
			animSpr->ChangeAnim(MarioAnims::CROUCH);
	}
}

void SlopedState::ProcessInputs()
{
}

void SlopedState::Update(float deltaTime)
{
}

void SlopedState::UpdateAnimation()
{
}

AirborneState::AirborneState(Player* ply)
	: PlayerState(ply)
{
}

void AirborneState::Resume()
{
}

void AirborneState::Initialise()
{
	auto ply = GetPlayer();

	if (ply)
		ply->SetYVelocity(-15);
}

void AirborneState::ProcessInputs()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();
	auto inputManager = GameManager::Get()->GetInputManager();

	if (!ply || !animSpr || !inputManager)
		return;

	if (inputManager->GetKeyState(Keys::LEFT_KEY))
	{
		if (ply->GetDirection())
			ply->SetDirection(false);

		//ply->DecrementXVelocity(m_physCtrl->GetXAcceleration());
	}

	if (inputManager->GetKeyState(Keys::RIGHT_KEY))
	{
		 if (!ply->GetDirection())
			 ply->SetDirection(true);

		 //ply->IncrementXVelocity(m_physCtrl->GetXAcceleration());
	}

	if (!inputManager->GetKeyState(Keys::JUMP_KEY))
	{
		/*if (ply->GetAirbourne() && ply->GetCantJump())
			ply->GetAirTimer()->SetTime(0);*/
	}

	if (!inputManager->GetKeyState(Keys::SJUMP_KEY))
	{
		/*if (ply->GetAirbourne() && ply->GetCantSpinJump())
			ply->GetAirTimer()->SetTime(0);*/
	}
}

void AirborneState::Update(float deltaTime)
{
	// nothing to update
}

void AirborneState::UpdateAnimation()
{
}

void CrouchedState::Initialise()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();

	if (!ply || !animSpr)
		return;

	ply->SetXVelocity(0);
	if (animSpr->GetCurrentAnim() != MarioAnims::CROUCH)
		animSpr->ChangeAnim(MarioAnims::CROUCH);
}

CrouchedState::CrouchedState(Player* ply)
	: PlayerState(ply)
{
}

void CrouchedState::Resume()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();

	if (!ply || !animSpr)
		return;

	if (animSpr->GetCurrentAnim() != MarioAnims::CROUCH)
		animSpr->ChangeAnim(MarioAnims::CROUCH);

	ply->SetXVelocity(0);
	PlayerState::Resume();
}

void CrouchedState::ProcessInputs()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();
	auto inputManager = GameManager::Get()->GetInputManager();

	if (!ply || !animSpr || !inputManager)
		return;

	if (inputManager->GetKeyState(Keys::JUMP_KEY))
	{
		if (!ply->GetCantJump())
			ply->SetAirbourne(true);
	}
	else
	{
		ply->SetCantJump(false);
	}

	if (inputManager->GetKeyState(Keys::SJUMP_KEY))
	{
		if (!ply->GetCantSpinJump())
		{
			if (animSpr->GetCurrentAnim() != MarioAnims::SPINJUMP)
				animSpr->ChangeAnim(MarioAnims::SPINJUMP);

			ply->SetAirbourne(true);
		}
	}
	else
	{
		ply->SetCantSpinJump(false);
	}
}

void CrouchedState::Update(float deltaTime)
{
	// nothing to update
}

void CrouchedState::UpdateAnimation()
{
}

DieingState::DieingState(Player* ply)
	: PlayerState(ply)
{
}

void DieingState::Initialise()
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();

	if (!ply || !animSpr)
		return;

	if (animSpr->GetCurrentAnim() != MarioAnims::DIE)
		animSpr->ChangeAnim(MarioAnims::DIE);

	ply->SetAirbourne(true);
}

void DieingState::ProcessInputs()
{
	// can't move when dieing
}

void DieingState::Update(float deltaTime)
{
	auto ply = GetPlayer();
	auto animSpr = GetAnimSpr();

	if (!ply || !animSpr)
		return;

	if (ply->GetAirbourne())
	{
		ply->GetAirTimer()->Update(deltaTime);
		if (ply->GetAirTimer()->CheckEnd())
			ply->SetAirbourne(false);

		/*if (m_physCtrl->GetPhysicsType() != PhysicsType::rise)
			m_physCtrl->SetAerial();*/

		//ply->DecrementYVelocity(m_physCtrl->GetYAcceleration());
		ply->Move(Vector2f(0, ply->GetYVelocity() * GameConstants::FPS * deltaTime));
	}
	else
	{
		/*if (m_physCtrl->GetPhysicsType() != PhysicsType::drop)
			m_physCtrl->SetAerial();*/

		//ply->IncrementYVelocity(m_physCtrl->GetYAcceleration());
		ply->Move(Vector2f(0, ply->GetYVelocity() * GameConstants::FPS * deltaTime));

		if (GameManager::Get()->GetCamera()->CheckVerticalBounds(ply->GetVolume()))
		{
			ply->Reset();
		}
	}
}

void DieingState::UpdateAnimation()
{
}