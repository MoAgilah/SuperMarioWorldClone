#include "PlayerState.h"

#include "../../GameObjects/Player.h"
#include "../../Utilities/GameMode.h"
#include <Drawables/SFSprite.h>
#include <Utilities/Utils.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <format>
#include <iostream>

MovementController PlayerState::m_movementCtrl = {};
float PlayerState::s_frameStep = 0.0f;

// Lateral
enum class LateralLoco
{
	Walk,
	Run,
	Sprint,
	EOL
};

enum class LateralAccl
{
	Imp
};

enum class LateralDecl
{
	Stop,
	Skid
};

// Inclined
enum class InclinedLoco
{
	Walk,
	Run,
	SmlDecWalk,
	LrgDecWalk,
	SmlDecRun,
	LrgDecRun,
	Slide
};

enum class InclinedAccl
{
	Imp,
	SldSmlInc,
	SldLrgInc
};

// Vertical
enum class VerticalLoco
{
	Low,
	Med,
	High,
	Max,
	Slam,
	Tap
};

enum class VerticalAccl
{
	High,
	Low
};

enum MovementStates
{
	Lateral,
	Crouched,
	Vertical,
	Inclined,
	Dieing
};

PlayerState::PlayerState(Player* ply)
	: IObjectState(ply)
{
	if (!m_initialisedCtrl)
	{
		MovementState lateral;

		lateral.AddVelocity(ToInt(LateralLoco::Walk), 1.5f);
		lateral.AddVelocity(ToInt(LateralLoco::Run), 2.5f);
		lateral.AddVelocity(ToInt(LateralLoco::Sprint), 3.5f);

		GameMode::m_mariosMaxSpdX = lateral.GetVelocity(ToInt(LateralLoco::Sprint));

		lateral.AddVelocity(ToInt(LateralLoco::EOL), 1.25f);

		lateral.AddAcceleration(ToInt(LateralAccl::Imp), 0.0546875f);
		lateral.SetCurrentAccel(ToInt(LateralAccl::Imp));

		lateral.AddDeceleration(ToInt(LateralDecl::Stop), 0.0546875f);
		lateral.AddDeceleration(ToInt(LateralDecl::Skid), 0.046875f);
		lateral.SetCurrentDecel(ToInt(LateralDecl::Stop));

		m_walkCap = lateral.GetVelocity(ToInt(LateralLoco::Walk));
		m_runCap = lateral.GetVelocity(ToInt(LateralLoco::Run));
		m_sprintCap = lateral.GetVelocity(ToInt(LateralLoco::Sprint));

		m_movementCtrl.AddMovementXState(Lateral, lateral);

		MovementState inclined;

		inclined.AddVelocity(ToInt(InclinedLoco::Walk), 0.8125f);
		inclined.AddVelocity(ToInt(InclinedLoco::Run), 1.375f);

		inclined.AddVelocity(ToInt(InclinedLoco::SmlDecWalk),
			inclined.GetVelocity(ToInt(InclinedLoco::Walk)) + 0.125f);
		inclined.AddVelocity(ToInt(InclinedLoco::LrgDecWalk),
			inclined.GetVelocity(ToInt(InclinedLoco::Walk)) + 0.1875f);

		inclined.AddVelocity(ToInt(InclinedLoco::SmlDecRun),
			inclined.GetVelocity(ToInt(InclinedLoco::Run)) + 0.125f);
		inclined.AddVelocity(ToInt(InclinedLoco::LrgDecRun),
			inclined.GetVelocity(ToInt(InclinedLoco::Run)) + 0.1875f);

		inclined.AddAcceleration(ToInt(InclinedAccl::Imp), 0.0546875f);
		inclined.AddAcceleration(ToInt(InclinedAccl::SldSmlInc), 0.0625f);
		inclined.AddAcceleration(ToInt(InclinedAccl::SldLrgInc), 0.125f);

		inclined.SetCurrentAccel(ToInt(InclinedAccl::Imp));

		m_movementCtrl.AddMovementXState(Inclined, inclined);

		MovementState vertical;

		vertical.AddVelocity(ToInt(VerticalLoco::Low), 3.4375f);		// X Velocity < 1 px/f (-Y)
		vertical.AddVelocity(ToInt(VerticalLoco::Med), 3.5625f);		// X Velocity > 1 px/f (-Y)
		vertical.AddVelocity(ToInt(VerticalLoco::High), 3.6875f);		// X Velocity > 2 px/f (-Y)
		vertical.AddVelocity(ToInt(VerticalLoco::Max), 3.9375f);		// X Velocity > 3 px/f (-Y)

		GameMode::m_marioMaxSpdY = lateral.GetVelocity(ToInt(VerticalLoco::Max));

		vertical.AddVelocity(ToInt(VerticalLoco::Slam), 4.f);			// -Y
		vertical.AddVelocity(ToInt(VerticalLoco::Tap), 3.f);			// -Y

		vertical.AddAcceleration(ToInt(VerticalAccl::High), 0.0625f);	//(A held, Y Vel < -2 px/f)
		vertical.AddAcceleration(ToInt(VerticalAccl::Low), 0.3125f);	//(Y Vel > -2 px / f)

		vertical.SetCurrentAccel(ToInt(VerticalAccl::High));

		m_movementCtrl.AddMovementYState(Vertical, vertical);

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
	DECL_GET_OR_RETURN(ply, GetPlayer());
	ply->SetActive(false);
}

void PlayerState::Resume()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	ply->SetActive(true);
}

void PlayerState::Update(float deltaTime)
{}

SFAnimatedSprite* PlayerState::GetAnimSpr()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable);
	if (spr)
		return spr;

	return nullptr;
}

Player* PlayerState::GetPlayer()
{
	auto ply = dynamic_cast<Player*>(m_gameObj);
	if (ply)
		return ply;

	return nullptr;
}

void PlayerState::MoveLeft(Player* ply)
{
	ENSURE_VALID(ply);

	if (ply->GetDirection())
		ply->SetDirection(false);

	auto& currMovState = m_movementCtrl.GetCurrentXState();

	ply->DecrementXVelocity(currMovState.GetCurrentAccel() * s_frameStep);

	if (ply->GetXVelocity() < -currMovState.GetCurrentVelLimit())
		ply->SetXVelocity(-currMovState.GetCurrentVelLimit());
}

void PlayerState::MoveRight(Player* ply)
{
	ENSURE_VALID(ply);

	if (!ply->GetDirection())
		ply->SetDirection(true);

	auto& currMovState = m_movementCtrl.GetCurrentXState();

	ply->IncrementXVelocity(currMovState.GetCurrentAccel() * s_frameStep);

	if (std::abs(ply->GetXVelocity()) > currMovState.GetCurrentVelLimit())
		ply->SetXVelocity(currMovState.GetCurrentVelLimit());
}

void PlayerState::MoveUp(Player* ply)
{
	ENSURE_VALID(ply);

	// Do NOT apply upward acceleration if not in the "airborne jump-hold" window
	if (!ply->GetAirbourne()) return;                 // only allow while airborne
	if (auto* t = ply->GetAirTimer(); t && t->CheckEnd()) return; // hold window ended

	auto& currMovState = m_movementCtrl.GetCurrentYState();

	ply->DecrementYVelocity(currMovState.GetCurrentAccel() * s_frameStep);

	if (currMovState.GetCurrentAccel() == ToInt(VerticalAccl::High))
	{
		if (ply->GetYVelocity() < -2)
			currMovState.SetCurrentAccel(ToInt(VerticalAccl::Low));
	}

	if(ply->GetYVelocity() < -currMovState.GetCurrentVelLimit())
		ply->SetYVelocity(-currMovState.GetCurrentVelLimit());
}

void PlayerState::MoveDown(Player* ply)
{
	ENSURE_VALID(ply);

	auto& currMovState = m_movementCtrl.GetCurrentYState();

	ply->IncrementYVelocity(currMovState.GetCurrentAccel() * s_frameStep);

	if (ply->GetYVelocity() > currMovState.GetCurrentVelLimit())
		ply->SetYVelocity(currMovState.GetCurrentVelLimit());
}

void PlayerState::InitiateJump(Player* ply)
{
	ENSURE_VALID(ply);

	if (ply->GetOnGround() && !ply->GetCantJump())
	{
		ply->SetAirbourne(true);
		ply->SetCantJump(true);
		ply->GetStateMgr()->PushState(new VerticalState(ply));
	}
}

void PlayerState::InititateSpinJump(Player* ply)
{
	ENSURE_VALID(ply);

	if (ply->GetOnGround() && !ply->GetCantSpinJump())
	{
		ply->SetAirbourne(true);
		ply->SetCantSpinJump(true);
		ply->GetStateMgr()->PushState(new VerticalState(ply, true));
	}
}

LateralState::LateralState(Player* ply)
	: PlayerState(ply)
{
}

void LateralState::Initialise()
{
	m_movementCtrl.ChangeMovementXState(Lateral);
	m_movementCtrl.ChangeMovementYState(Vertical);

	m_movementCtrl.GetCurrentXState().SetCurrentAccel(ToInt(VerticalAccl::High));
}

void LateralState::Resume()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	PlayerState::Resume();
}

void LateralState::ProcessInputs()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(inputManager, gameMgr->GetInputManager());

	if (inputManager->GetKeyState(Keys::LEFT_KEY))
		MoveLeft(ply);

	if (inputManager->GetKeyState(Keys::RIGHT_KEY))
		MoveRight(ply);

	if (!inputManager->GetKeyState(Keys::LEFT_KEY) && !inputManager->GetKeyState(Keys::RIGHT_KEY))
	{
		if (inputManager->GetKeyState(Keys::UP_KEY))
			animSpr->EnsureAnim(MarioAnims::LOOKUP);
		else
			animSpr->EnsureAnim(MarioAnims::IDLE);
	}

	if (inputManager->GetKeyState(Keys::DOWN_KEY))
	{
		ply->SetIsCrouched(true);
		ply->GetStateMgr()->PushState(new CrouchedState(ply));
	}

	m_runKeyHeld = inputManager->GetKeyState(Keys::RUN_KEY);

	if (inputManager->GetKeyState(Keys::JUMP_KEY))
	{
		InitiateJump(ply);
	}
	else
	{
		ply->SetCantJump(false);
	}

	if (inputManager->GetKeyState(Keys::SJUMP_KEY))
	{
		InititateSpinJump(ply);
	}
	else
	{
		ply->SetCantSpinJump(false);
	}
}

void LateralState::Update(float deltaTime)
{
	ProcessInputs();
	UpdateAnimation();

	if (m_runKeyHeld)
	{
		DECL_GET_OR_RETURN(ply, GetPlayer());

		auto velX = ply->GetXVelocity();

		if (velX < m_walkCap)
		{
			m_movementCtrl.GetCurrentXState().SetCurrentVelLimit(ToInt(LateralLoco::Walk));
		}
		if (velX >= m_walkCap && velX < m_runCap)
		{
			m_movementCtrl.GetCurrentXState().SetCurrentVelLimit(ToInt(LateralLoco::Run));
		}
		if (velX >= m_runCap)
		{
			m_movementCtrl.GetCurrentXState().SetCurrentVelLimit(ToInt(LateralLoco::Sprint));
		}
	}
	else
	{
		m_movementCtrl.GetCurrentXState().SetCurrentVelLimit(ToInt(LateralLoco::Walk));
	}
}

void LateralState::UpdateAnimation()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	auto& currMovState = m_movementCtrl.GetCurrentXState();

	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(inputManager, gameMgr->GetInputManager());

	if (inputManager->GetKeyState(Keys::LEFT_KEY) || inputManager->GetKeyState(Keys::RIGHT_KEY))
	{
		if (m_runKeyHeld)
		{
			auto currVelLimit = currMovState.GetCurrentVelLimit();

			if (currVelLimit == m_runCap)
			{
				if (animSpr->GetCurrAnimSpeed() != 0.75f)
					animSpr->UpdateAnimSpeed(0.75f);

				animSpr->EnsureAnim(MarioAnims::MOVING);
			}
			else if (currVelLimit == m_sprintCap)
			{
				if (animSpr->GetCurrAnimSpeed() != 0.5f)
					animSpr->UpdateAnimSpeed(0.5f);

				animSpr->EnsureAnim(MarioAnims::RUNNING);
			}
		}
		else
		{
			if (animSpr->GetCurrAnimSpeed() != 0.5f)
				animSpr->UpdateAnimSpeed(0.5f);

			animSpr->EnsureAnim(MarioAnims::MOVING);
		}
	}
}

CrouchedState::CrouchedState(Player* ply)
	: PlayerState(ply)
{
}

void CrouchedState::Resume()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	m_movementCtrl.ChangeMovementXState(Lateral);

	ply->SetIsCrouched(true);

	animSpr->EnsureAnim(MarioAnims::CROUCH);

	ply->SetXVelocity(0);
	PlayerState::Resume();
}

void CrouchedState::Initialise()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	m_movementCtrl.ChangeMovementXState(Lateral);

	ply->SetXVelocity(0);
	animSpr->EnsureAnim(MarioAnims::CROUCH);
}

void CrouchedState::ProcessInputs()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(inputManager, gameMgr->GetInputManager());

	if (!inputManager->GetKeyState(Keys::DOWN_KEY))
		ply->GetStateMgr()->PopState();

	if (inputManager->GetKeyState(Keys::JUMP_KEY))
	{
		InitiateJump(ply);
	}
	else
	{
		ply->SetCantJump(false);
	}

	if (inputManager->GetKeyState(Keys::SJUMP_KEY))
	{
		InititateSpinJump(ply);
	}
	else
	{
		ply->SetCantSpinJump(false);
	}
}

void CrouchedState::Update(float deltaTime)
{
	ProcessInputs();
	UpdateAnimation();
}

void CrouchedState::UpdateAnimation()
{
}

InclinedState::InclinedState(Player* ply)
	: PlayerState(ply)
{
}

void InclinedState::Resume()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	m_movementCtrl.ChangeMovementXState(Inclined);
}

void InclinedState::Initialise()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	m_movementCtrl.ChangeMovementXState(Inclined);
}

void InclinedState::ProcessInputs()
{
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(inputManager, gameMgr->GetInputManager());

	if (inputManager->GetKeyState(Keys::LEFT_KEY))
		MoveLeft(ply);

	if (inputManager->GetKeyState(Keys::RIGHT_KEY))
		MoveRight(ply);

	if (inputManager->GetKeyState(Keys::JUMP_KEY))
	{
		InitiateJump(ply);
	}
	else
	{
		ply->SetCantJump(false);
	}

	if (inputManager->GetKeyState(Keys::SJUMP_KEY))
	{
		InititateSpinJump(ply);
	}
	else
	{
		ply->SetCantSpinJump(false);
	}
}

void InclinedState::Update(float deltaTime)
{
	ProcessInputs();
	UpdateAnimation();
}

void InclinedState::UpdateAnimation()
{
}

VerticalState::VerticalState(Player* ply, bool spinJump, bool fromFall)
	: PlayerState(ply), m_spinJump(spinJump), m_fromFall(fromFall)
{
	m_airTimer = ply->GetAirTimer();
	ENSURE_VALID(m_airTimer);
}

void VerticalState::Resume()
{
}

void VerticalState::Initialise()
{
	ENSURE_VALID(m_airTimer);
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	auto& currMovState = m_movementCtrl.GetCurrentYState();

	animSpr->UpdateAnimSpeed(0.5f);

	if (m_fromFall)
	{
		animSpr->EnsureAnim(MarioAnims::FALL);
		m_airTimer->SetCurrTime(0);
		currMovState.SetCurrentVelLimit(ToInt(VerticalLoco::Max));
	}
	else
	{
		// normal jump/spin-jump path (existing behavior)
		if (m_spinJump)
			animSpr->EnsureAnim(MarioAnims::SPINJUMP);
		else if (animSpr->GetCurrentAnim() != MarioAnims::CROUCH)
			animSpr->EnsureAnim(MarioAnims::JUMP);

		// pick vertical speed cap from current X speed
		float xVel = ply->GetXVelocity();
		if (xVel < Speed::Low)
			currMovState.SetCurrentVelLimit(ToInt(VerticalLoco::Low));
		else if (xVel > Speed::Low && xVel <= Speed::Med)
			currMovState.SetCurrentVelLimit(ToInt(VerticalLoco::Med));
		else if (xVel > Speed::Med && xVel <= Speed::High)
			currMovState.SetCurrentVelLimit(ToInt(VerticalLoco::High));
		else
			currMovState.SetCurrentVelLimit(ToInt(VerticalLoco::Max));

		m_airTimer->RestartTimer();
	}
}

void VerticalState::ProcessInputs()
{
	ENSURE_VALID(m_airTimer);
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(inputManager, gameMgr->GetInputManager());

	if (inputManager->GetKeyState(Keys::LEFT_KEY))
		MoveLeft(ply);

	if (inputManager->GetKeyState(Keys::RIGHT_KEY))
		MoveRight(ply);

	if (inputManager->GetKeyState(Keys::JUMP_KEY))
		MoveUp(ply);

	if (inputManager->GetKeyState(Keys::SJUMP_KEY))
		MoveUp(ply);

	bool endAirTime = false;
	if (m_spinJump)
	{
		endAirTime = !inputManager->GetKeyState(Keys::SJUMP_KEY)
			&& ply->GetAirbourne()
			&& ply->GetCantSpinJump();
	}
	else
	{
		endAirTime = !inputManager->GetKeyState(Keys::JUMP_KEY)
			&& ply->GetAirbourne()
			&& ply->GetCantJump();
	}

	if (endAirTime)
		m_airTimer->SetCurrTime(0);
}

void VerticalState::Update(float deltaTime)
{
	ProcessInputs();
	UpdateAnimation();

	ENSURE_VALID(m_airTimer);
	DECL_GET_OR_RETURN(ply, GetPlayer());

	auto& movementState = m_movementCtrl.GetCurrentYState();

	auto yVel = ply->GetYVelocity();
	if (yVel < Speed::Med)
	{
		movementState.SetCurrentAccel(ToInt(VerticalAccl::High));
	}
	else if (yVel > Speed::Med)
	{
		movementState.SetCurrentAccel(ToInt(VerticalAccl::Low));
	}

	m_airTimer->Update(deltaTime);
	if (m_airTimer->CheckEnd())
		MoveDown(ply);
}

void VerticalState::UpdateAnimation()
{
	ENSURE_VALID(m_airTimer);
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	if (m_airTimer->CheckEnd())
	{
		if (!m_spinJump && animSpr->GetCurrentAnim() != MarioAnims::CROUCH)
			animSpr->EnsureAnim(MarioAnims::FALL);
	}
}

DieingState::DieingState(Player* ply)
	: PlayerState(ply)
{
	m_airTimer = ply->GetAirTimer();
	ENSURE_VALID(m_airTimer);
}

void DieingState::Initialise()
{
	ENSURE_VALID(m_airTimer);
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());

	animSpr->EnsureAnim(MarioAnims::DIE);

	ply->SetAirbourne(true);
}

void DieingState::ProcessInputs()
{
	// can't move when dieing
}

void DieingState::Update(float deltaTime)
{
	ENSURE_VALID(m_airTimer);
	DECL_GET_OR_RETURN(ply, GetPlayer());
	DECL_GET_OR_RETURN(animSpr, GetAnimSpr());
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(camera, gameMgr->GetCamera());

	if (ply->GetAirbourne())
	{
		m_airTimer->Update(deltaTime);
		if (m_airTimer->CheckEnd())
			ply->SetAirbourne(false);

		MoveUp(ply);
	}
	else
	{
		MoveDown(ply);

		if (camera->CheckVerticalBounds(ply->GetVolume()))
			ply->Reset();
	}
}

void DieingState::UpdateAnimation()
{
}