#include "Koopa.h"

#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

Koopa::Koopa(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Koopa",3,3,GameConstants::FPS, false, 0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(17, 12)))
{
	m_dynType = typeid(*this);

	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.25f);
	SetSpeedY(GameMode::m_marioMaxSpdY);

	ENSURE_VALID(m_volume);
	m_volume->Update(GetPosition());

	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({ 2, 3, 1 });
	spr->ChangeAnim(KoopaAnims::WALK);
}

void Koopa::Reset()
{
	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(KoopaAnims::WALK);

	Enemy::Reset();
}

void Koopa::Die()
{
	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(KoopaAnims::COMPRESS);

	SetTimeLeftActive(0.5f);
}

void Koopa::Animate(float deltaTime)
{
	DECL_GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));
	const float step = GameConstants::FPS * deltaTime;

	spr->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(GetSpeedX());
	}
	else
	{
		SetXVelocity(-GetSpeedX());
	}

	if (GetOnGround())
	{
		if (GetOnSlope())
		{
			if (spr->GetCurrentAnim() != KoopaAnims::NOSEDIVE)
				spr->ChangeAnim(KoopaAnims::NOSEDIVE);

			if (!GetXVelocity())
			{
				if (GetShouldSlideLeft())
					SetSlideLeft(true);

				if (GetShouldSlideRight())
					SetSlideRight(true);
			}

			if (GetSlideLeft() || GetSlideRight())
			{
				if (GetSlideLeft())
					DecrementXVelocity(GameConstants::Gravity * step);

				if (GetSlideRight())
					IncrementXVelocity(GameConstants::Gravity * step);
			}
		}
		else
		{
			if (spr->GetCurrentAnim() == KoopaAnims::NOSEDIVE)
				spr->ChangeAnim(KoopaAnims::WALK);

			SetSlideLeft(false);
			SetSlideRight(false);
			SetShouldSlideLeft(false);
			SetShouldSlideRight(false);
		}

		SetYVelocity(0);
	}
	else
	{
		IncrementYVelocity(GameConstants::Gravity * step);
	}

	if (HasLifes())
	{
		DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
		DECL_GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * step, 0);
			colMgr->ProcessCollisions(this);
		}

		GameMode::CheckForHorizontalBounds(deltaTime, this);

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * step);
			colMgr->ProcessCollisions(this);
		}
	}
}