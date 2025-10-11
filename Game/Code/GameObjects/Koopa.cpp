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

	spr->Update(deltaTime);

	if (HasLifes())
	{
		SetPrevPosition(GetPosition());

		if (GetOnGround())
		{
			if (GetOnSlope())
			{
				spr->EnsureAnim(KoopaAnims::NOSEDIVE);

				if (!GetSlideLeft() || !GetSlideRight())
				{
					if (GetDirection())
					{

						SetShouldSlideRight(true);
					}
					else
					{
						SetShouldSlideLeft(true);
					}
				}
				else
				{
					if (GetSlideLeft())
						DecrementXVelocity(GameConstants::Gravity);

					if (GetSlideRight())
						IncrementXVelocity(GameConstants::Gravity);
				}
			}
			else
			{
				if (spr->GetCurrentAnim() == KoopaAnims::NOSEDIVE)
					spr->ChangeAnim(KoopaAnims::WALK);

				SetSlideLeft(false);
				SetSlideRight(false);
			}

			if (GetYVelocity() != 0)
				SetYVelocity(0);
		}
		else
		{
			if (GetYVelocity() < GameMode::m_marioMaxSpdY)
				IncrementYVelocity(GameConstants::Gravity);
		}

		DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
		DECL_GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
			colMgr->ProcessCollisions(this);
		}

		GameMode::CheckForHorizontalBounds(deltaTime, this);

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
			colMgr->ProcessCollisions(this);
		}
	}
}