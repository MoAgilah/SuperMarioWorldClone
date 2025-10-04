#include "Koopa.h"

#include "../Utilities/GameMode.h"
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Interface/Collisions/ICollisionManager.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>

Koopa::Koopa(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Koopa",3,3,GameConstants::FPS, false, 0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(17, 12),Vector2f()))
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	m_volume->Update(GetPosition());

	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.25f);
	SetSpeedY(GameMode::m_marioMaxSpdY);

	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
	{
		spr->SetFrames({ 2, 3, 1 });
		spr->ChangeAnim(KoopaAnims::WALK);
	}
}

void Koopa::Reset()
{
	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
		spr->EnsureAnim(KoopaAnims::WALK);

	Enemy::Reset();
}

void Koopa::SetDirection(bool dir)
{
	Enemy::SetDirection(dir);
	if (dir)
		SetXVelocity(GetSpeedX());
	else
		SetXVelocity(-GetSpeedX());
}

void Koopa::Die()
{
	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
		spr->EnsureAnim(KoopaAnims::COMPRESS);

	SetTimeLeftActive(0.5f);
}

void Koopa::Animate(float deltaTime)
{
	auto spr = GetAnimatedSprite(m_drawable.get());
	auto* colMgr = GameManager::Get()->GetCollisionMgr();

	if (!spr)
		return;

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

		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
			colMgr->ProcessCollisions(this);
		}

		/*CheckForHorizontalBounds(deltaTime);*/

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
			colMgr->ProcessCollisions(this);
		}
	}
}