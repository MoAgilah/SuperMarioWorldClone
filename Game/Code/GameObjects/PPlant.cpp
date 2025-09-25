#include "PPlant.h"

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

	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({2});
}

void PPlant::Reset()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if(spr->GetCurrentAnim() != PPlantAnims::SINK)
			spr->ChangeAnim(PPlantAnims::SINK);
	}

	Enemy::Reset();
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

	//PhysicsController* physCtrl = GetPhysicsController();

	spr->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetYVelocity(GameConstants::ObjectSpeed);
	}
	else
	{
		SetYVelocity(-GameConstants::ObjectSpeed);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}

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
		SetAirTime(/*GameConstants::MaxAirTime*/1+.3f);
	}
}
