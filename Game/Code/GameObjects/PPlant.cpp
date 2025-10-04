#include "PPlant.h"

#include "../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>

PPlant::PPlant(const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Chuck", 1, 2, GameConstants::FPS, false, 0.5f),
	std::make_shared<BoundingBox<SFRect>>(Vector2f(14, 19), Vector2f()))
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	m_volume->Update(GetPosition());

	SetSpeedX(0);
	SetSpeedY(GameMode::m_mariosMaxSpdX * 0.25f);
	SetAirTime(1.3f);

	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
		spr->SetFrames({2});
}

void PPlant::Reset()
{
	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
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
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;

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

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}
}
