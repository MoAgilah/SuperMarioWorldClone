#include "Chuck.h"

#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>

Chuck::Chuck(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Chuck",5,7,GameConstants::FPS, false,0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(24.f,25.f),Vector2f())), m_waitTimer(0)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	m_volume->Update(GetPosition());
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({ 3, 1, 1, 7, 3 });
}

void Chuck::Reset()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != ChuckAnims::BOUNCE)
			spr->ChangeAnim(ChuckAnims::BOUNCE);
	}
	Enemy::Reset();
}

void Chuck::Die()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != ChuckAnims::WHIPLASH)
			spr->ChangeAnim(ChuckAnims::WHIPLASH);
	}
	SetTimeLeftActive(0.5f);
}

void Chuck::DecrementLife()
{
	if (GetInvulnerabe())
		Enemy::DecrementLife();

	if (GetIsAlive())
	{
		m_tookHit = true;
		SetInvulnerability(true);
		auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
		if (spr)
		{
			if (spr->GetCurrentAnim() != ChuckAnims::HIT)
				spr->ChangeAnim(ChuckAnims::HIT);
		}
	}
}

void Chuck::Animate(float deltaTime)
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;
	/*PhysicsController* physCtrl = GetPhysicsController();*/

	spr->Update(deltaTime);

	if (m_tookHit)
	{
		if (GetAirbourne())
		{
			/*if (physCtrl->GetPhysicsType() != PhysicsType::drop)
				physCtrl->SetFalling();*/

			//IncrementYVelocity(physCtrl->GetYAcceleration());
		}
		else
		{
			if (spr->PlayedNumTimes(2))
			{
				m_tookHit = false;
				SetInvulnerability(false);
			}
		}

		if (GetOnGround())
			SetAirbourne(false);
	}
	else
	{
		if (GetAirbourne())
		{
			m_waitTimer.Update(deltaTime);
			if (m_waitTimer.CheckEnd())
			{
				/*if (physCtrl->GetPhysicsType() != PhysicsType::rise)
					physCtrl->SetAerial();*/

				//DecrementYVelocity(physCtrl->GetYAcceleration());
				GetAirTimer()->Update(deltaTime);
				if (spr->GetCurrentAnim() != ChuckAnims::LEAP)
					spr->ChangeAnim(ChuckAnims::LEAP);
			}
			else
			{
				SetYVelocity(0);
			}
		}
		else
		{
			/*if (physCtrl->GetPhysicsType() != PhysicsType::drop)
				physCtrl->SetFalling();*/

			//IncrementYVelocity(physCtrl->GetYAcceleration());
		}

		auto currentPos = GetPosition();

		if (GetAirTimer()->CheckEnd())
		{
			if (spr->GetCurrentAnim() != ChuckAnims::CLAP)
				spr->ChangeAnim(ChuckAnims::CLAP);
			SetAirbourne(false);
		}

		if (GetOnGround())
		{
			if (!GetAirbourne())
			{
				if (spr->GetCurrentAnim() != ChuckAnims::BOUNCE)
					spr->ChangeAnim(ChuckAnims::BOUNCE);
				m_waitTimer.SetCurrTime(0.5f);
				SetAirTime(0.75f);
				SetAirbourne(true);
			}
		}
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}
}