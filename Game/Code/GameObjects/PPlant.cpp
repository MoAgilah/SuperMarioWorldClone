#include "PPlant.h"

#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

PPlant::PPlant(const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("PPlant", 1, 2, GameConstants::FPS, false, 0.5f),
	std::make_shared<BoundingBox<SFRect>>(Vector2f(14, 19)))
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	SetSpeedX(0);
	SetSpeedY(GameMode::m_mariosMaxSpdX * 0.25f);
	SetAirTime(1.3f);

	ENSURE_VALID(m_volume);
	m_volume->Update(GetPosition());

	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({2});
}

void PPlant::Reset()
{
	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(PPlantAnims::SINK);

	Enemy::Reset();
}

void PPlant::SetDirection(bool dir)
{
	Enemy::SetDirection(dir);
	if (GetDirection())
		SetYVelocity(GetSpeedY());
	else
		SetYVelocity(GetSpeedY());
}

void PPlant::Die()
{
	// no way to destroy currently, requires fire plant mario, shell toss, or star power not currently implemented
}

void PPlant::Animate(float deltaTime)
{
	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));
	ENSURE_VALID(GetAirTimer());

	spr->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetAirbourne())
	{
		SetDirection(false);
		GetAirTimer()->Update(deltaTime);
	}
	else
	{
		SetDirection(true);
	}

	if (GetPosition().y > GetInitialPosition().y)
		SetAirbourne(true);

	if (GetAirTimer()->CheckEnd())
	{
		SetAirbourne(false);
		GetAirTimer()->RestartTimer();
	}

	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		colMgr->ProcessCollisions(this);
	}
}
