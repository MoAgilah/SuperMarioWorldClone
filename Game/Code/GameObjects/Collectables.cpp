#include "Collectables.h"

#include "../GameObjects/Player.h"
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>

int YCoin::s_collected = 1;

Coin::Coin(const Vector2f& initPos)
	: StaticCollectable(std::make_shared<SFAnimatedSprite>("Coin", 1, 4, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(12, 16), Vector2f()), initPos)
{
	m_volume->Update(initPos);
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({ 4 });
}

void Coin::Update(float deltaTime)
{
	m_drawable->Update(deltaTime);
}

void Coin::Collect(GameObject* object)
{
	/*SetCollected();
	player->IncreaseCoins(1);*/
}

YCoin::YCoin(const Vector2f& initPos)
	: StaticCollectable(std::make_shared<SFAnimatedSprite>("YCoin", 1, 6, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(16, 25), Vector2f()), initPos)
{
	m_volume->Update(initPos);
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({ 6 });
}

void YCoin::Update(float deltaTime)
{
	m_drawable->Update(deltaTime);
}

void YCoin::Reset()
{
	GameObject::Reset();
	s_collected = 1;
}

void YCoin::Collect(GameObject* obj)
{
	/*if (s_collected < 8)
		s_collected *= 2;

	SetCollected();
	player->IncreaseCoins(s_collected * 1000);*/
}

CheckPoint::CheckPoint(const Vector2f& initPos)
	: StaticCollectable(std::make_shared<SFSprite>("ChkPnt"), std::make_shared<BoundingBox<SFRect>>(Vector2f(20, 3), Vector2f()), initPos)
{
	m_volume->Update(initPos);
}

void CheckPoint::Update(float deltaTime)
{
	// nothing to update
}

void CheckPoint::Collect(GameObject* obj)
{
	/*SetCollected();
	player->SetIsSuper(true);
	player->SetSpawnLoc(GetPosition());*/
}

Mushroom::Mushroom(const Vector2f& initPos)
	: DynamicCollectable(std::make_shared<SFSprite>("Shroom"), std::make_shared<BoundingBox<SFRect>>(Vector2f(13, 10), Vector2f()), initPos)
{
	m_volume->Update(initPos);
}

void Mushroom::Update(float deltaTime)
{
	auto colMgr = GameManager::Get()->GetCollisionMgr();
	//PhysicsController* physCtrl = GetPhysicsController();

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

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		colMgr->ProcessCollisions(this);
	}

	if (GetXVelocity() != 0)
	{
		Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
		colMgr->ProcessCollisions(this);
	}

	//CheckForHorizontalBounds(deltaTime);
}

void Mushroom::Collect(GameObject* obj)
{
	/*SetCollected();
	player->SetIsSuper(true);*/
}

Goal::Goal(const Vector2f& initPos)
	: DynamicCollectable(std::make_shared<SFSprite>("Goal"), std::make_shared<BoundingBox<SFRect>>(Vector2f(25, 8), Vector2f()), initPos)
	, m_airTimer(0)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetOnGround(true);
	m_volume->Update(initPos);
}

void Goal::Update(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetOnGround())
	{
		m_airTimer.Update(deltaTime);
		SetYVelocity(-GameConstants::ObjectSpeed);
	}
	else
	{
		SetYVelocity(GameConstants::ObjectSpeed);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}

	if (m_airTimer.CheckEnd())
	{
		m_airTimer.SetCurrTime(c_maxTravelTime);
		SetOnGround(false);
	}
}

void Goal::Collect(GameObject* obj)
{
	/*SetCollected();
	player->SetGoalHit(true);*/
}