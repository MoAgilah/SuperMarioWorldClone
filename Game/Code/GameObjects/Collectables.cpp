#include "Collectables.h"

#include "../GameObjects/Player.h"
#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

int YCoin::s_collected = 1;

Coin::Coin(const Vector2f& initPos)
	: StaticCollectable(std::make_shared<SFAnimatedSprite>("Coin", 1, 4, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(12, 16)), initPos)
{
	ENSURE_VALID(m_volume);
	m_volume->Update(initPos);

	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({ 4 });
}

void Coin::Update(float deltaTime)
{
	ENSURE_VALID(m_drawable);
	m_drawable->Update(deltaTime);
}

void Coin::Collect(GameObject* object)
{
	/*SetCollected();
	player->IncreaseCoins(1);*/
}

YCoin::YCoin(const Vector2f& initPos)
	: StaticCollectable(std::make_shared<SFAnimatedSprite>("YCoin", 1, 6, GameConstants::FPS, false, 0.5f), std::make_shared<BoundingBox<SFRect>>(Vector2f(16, 25)), initPos)
{
	ENSURE_VALID(m_volume);
	m_volume->Update(initPos);

	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({ 6 });
}

void YCoin::Update(float deltaTime)
{
	ENSURE_VALID(m_drawable);
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
	: StaticCollectable(std::make_shared<SFSprite>("ChkPnt"), std::make_shared<BoundingBox<SFRect>>(Vector2f(20, 3)), initPos)
{
	ENSURE_VALID(m_volume);
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
	: DynamicCollectable(std::make_shared<SFSprite>("Shroom"), std::make_shared<BoundingBox<SFRect>>(Vector2f(13, 10)), initPos)
{
	ENSURE_VALID(m_volume);
	m_volume->Update(initPos);
}

void Mushroom::Update(float deltaTime)
{
	GET_OR_RETURN(gameMgr, GameManager::Get());
	GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

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

	GameMode::CheckForHorizontalBounds(deltaTime, this);
}

void Mushroom::Collect(GameObject* obj)
{
	/*SetCollected();
	player->SetIsSuper(true);*/
}

Goal::Goal(const Vector2f& initPos)
	: DynamicCollectable(std::make_shared<SFSprite>("Goal"), std::make_shared<BoundingBox<SFRect>>(Vector2f(25, 8)), initPos)
	, m_airTimer(0)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetOnGround(true);

	ENSURE_VALID(m_volume);
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

	GET_OR_RETURN(gameMgr, GameManager::Get());
	GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		colMgr->ProcessCollisions(this);
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