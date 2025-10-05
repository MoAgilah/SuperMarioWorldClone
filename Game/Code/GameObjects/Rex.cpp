#include "Rex.h"

#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

Rex::Rex(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Rex", 4, 3, GameConstants::FPS, false, 0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(10, 26)), 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.12f);
	SetSpeedY(GameMode::m_marioMaxSpdY);

	ENSURE_VALID(m_volume);
	m_volume->Update(GetPosition());

	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({ 2, 3, 2, 1 });
}

void Rex::Reset()
{
	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.12f);

	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(RexAnims::WALKTALL);

	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
	if (box)
		box->Reset(Vector2f(10, 28));

	Enemy::Reset();
}

void Rex::Die()
{
	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(RexAnims::FLATTEN);

	SetTimeLeftActive(1.f);
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

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
	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->Update(deltaTime);

	if (HasLifes())
	{
		SetPrevPosition(GetPosition());

		if (m_transitioning)
		{
			if (spr->PlayedNumTimes(1))
			{
				spr->EnsureAnim(RexAnims::WALKSHORT);
				m_transitioning = false;
			}
		}

		if (GetOnGround())
		{
			SetYVelocity(0);
		}
		else
		{
			if (GetYVelocity() < GameMode::m_marioMaxSpdY)
				IncrementYVelocity(GameConstants::Gravity);
		}

		GET_OR_RETURN(gameMgr, GameManager::Get());
		GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

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