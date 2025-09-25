#include "Koopa.h"

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
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		spr->SetFrames({ 2, 3, 1 });
		spr->ChangeAnim(KoopaAnims::WALK);
	}
}

void Koopa::Reset()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != KoopaAnims::WALK)
			spr->ChangeAnim(KoopaAnims::WALK);
	}
	Enemy::Reset();
}

void Koopa::Die()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != KoopaAnims::COMPRESS)
			spr->ChangeAnim(KoopaAnims::COMPRESS);
	}
	SetTimeLeftActive(0.5f);
}

void Koopa::Animate(float deltaTime)
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;
	//PhysicsController* physCtrl = GetPhysicsController();
	auto* colMgr = GameManager::Get()->GetCollisionMgr();
	if (colMgr)
		return;

	spr->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(GameConstants::ObjectSpeed);
	}
	else
	{
		SetXVelocity(-GameConstants::ObjectSpeed);
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
				/*if (GetSlideLeft())
					DecrementXVelocity(physCtrl->GetXAcceleration());

				if (GetSlideRight())
					IncrementXVelocity(physCtrl->GetXAcceleration());*/
			}
		}
		else
		{
			/*if (physCtrl->GetPhysicsType() == PhysicsType::drop)
				physCtrl->SetWalking();

			if (animSpr->GetCurrentAnim() == KoopaAnims::NOSEDIVE)
				animSpr->ChangeAnim(KoopaAnims::WALK);*/

			SetSlideLeft(false);
			SetSlideRight(false);
			SetShouldSlideLeft(false);
			SetShouldSlideRight(false);
		}

		SetYVelocity(0);
	}
	else
	{
		/*if (physCtrl->GetPhysicsType() != PhysicsType::drop)
			physCtrl->SetFalling();*/

		IncrementYVelocity(GameConstants::Gravity);
	}

	if (HasLifes())
	{
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