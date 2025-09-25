#include "Rex.h"

#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>

Rex::Rex(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Rex", 4, 3, GameConstants::FPS, false, 0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(10, 26), Vector2f()), 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	m_volume->Update(GetPosition());
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({ 2, 3, 2, 1 });
}

void Rex::Reset()
{

	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != RexAnims::WALKTALL)
			spr->ChangeAnim(RexAnims::WALKTALL);
	}

	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
	if (box)
		box->Reset(Vector2f(10, 28));

	Enemy::Reset();
}

void Rex::Die()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != RexAnims::FLATTEN)
			spr->ChangeAnim(RexAnims::FLATTEN);
	}
	SetTimeLeftActive(1.f);
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
		if (spr)
		{
			if (spr->GetCurrentAnim() != RexAnims::TRANSITION)
				spr->ChangeAnim(RexAnims::TRANSITION);
		}

		auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
		if (box)
			box->Reset(Vector2f(14, 16));

		Move((GetDirection() ? -1.f : 1.f) * 3.f, m_heightDiff);

		m_transitioning = true;
		m_squished = true;
	}

	Enemy::DecrementLife();
}

void Rex::Animate(float deltaTime)
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;

	//PhysicsController* physCtrl = GetPhysicsController();
	auto colMgr = GameManager::Get()->GetCollisionMgr();

	spr->Update(deltaTime);

	if (m_transitioning)
	{
		if (spr->PlayedNumTimes(1))
		{
			if (spr->GetCurrentAnim() != RexAnims::WALKSHORT)
				spr->ChangeAnim(RexAnims::WALKSHORT);
			m_transitioning = false;
		}
	}

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

		//CheckForHorizontalBounds(deltaTime);

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
			colMgr->ProcessCollisions(this);
		}
	}
}