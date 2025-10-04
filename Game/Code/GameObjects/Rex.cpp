#include "Rex.h"

#include "../Utilities/GameMode.h"
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

	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.12f);
	SetSpeedY(GameMode::m_marioMaxSpdY);

	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
		spr->SetFrames({ 2, 3, 2, 1 });
}

void Rex::Reset()
{

	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.12f);

	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
		spr->EnsureAnim(RexAnims::WALKTALL);

	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
	if (box)
		box->Reset(Vector2f(10, 28));

	Enemy::Reset();
}

void Rex::Die()
{
	auto spr = GetAnimatedSprite(m_drawable.get());
	if (spr)
		spr->EnsureAnim(RexAnims::FLATTEN);

	SetTimeLeftActive(1.f);
}

void Rex::SetDirection(bool dir)
{
	Enemy::SetDirection(dir);
	if (dir)
		SetXVelocity(GetSpeedX());
	else
		SetXVelocity(-GetSpeedX());
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		auto spr = GetAnimatedSprite(m_drawable.get());
		if (spr)
			spr->EnsureAnim(RexAnims::TRANSITION);

		auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
		if (box)
			box->Reset(Vector2f(14, 16));

		Move((GetDirection() ? -1.f : 1.f) * 3.f, m_heightDiff);

		SetSpeedX(GameMode::m_mariosMaxSpdX * 0.19f);

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

	spr->Update(deltaTime);

	if (m_transitioning)
	{
		if (spr->PlayedNumTimes(1))
		{
			spr->EnsureAnim(RexAnims::WALKSHORT);
			m_transitioning = false;
		}
	}

	SetPrevPosition(GetPosition());

	if (GetOnGround())
	{
		SetYVelocity(0);
	}
	else
	{
		if (GetYVelocity() < GameMode::m_marioMaxSpdY)
			IncrementYVelocity(GameConstants::Gravity);
	}

	if (HasLifes())
	{
		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
			GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
		}

		//CheckForHorizontalBounds(deltaTime);

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
			GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
		}
	}
}