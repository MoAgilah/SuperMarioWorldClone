#include "Bill.h"

#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingCapsule.h>
#include <Engine/Collisions/BoundingHalfCapsule.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>
#include <memory>

Bill::Bill(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFSprite>("Bill"), std::make_shared<BoundingCapsule<SFCapsule>>(24.f, 44.f, 90.f))
{
	m_dynType = typeid(*this);

	SetInitialDirection(dir);
	SetInitialPosition(initPos);
	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.75f);
	SetSpeedY(GameMode::m_marioMaxSpdY);

	ENSURE_VALID(m_volume);

	DECL_GET_OR_RETURN(fullCap, dynamic_cast<BoundingCapsule<SFCapsule>*>(m_volume.get()));

	HalfCapsule<SFCapsule>::Which which =
		GetInitialDirection() ? HalfCapsule<SFCapsule>::Which::End   // moving right → nose at "end"
		: HalfCapsule<SFCapsule>::Which::Start; // moving left  → nose at "start"

	m_halfCap = std::make_shared<HalfCapsule<SFCapsule>>(fullCap, which);

	ENSURE_VALID(m_halfCap);
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());

	UpdateBody();
}

bool Bill::Intersects(IGameObject* obj)
{
	ENSURE_VALID_RET(obj, false);
	ENSURE_VALID_RET(m_halfCap, false);

	return m_halfCap->Intersects(obj->GetVolume());
}

bool Bill::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	ENSURE_VALID_RET(obj, false);
	ENSURE_VALID_RET(m_halfCap, false);

	return m_halfCap->IntersectsMoving(obj->GetVolume(), GetVelocity(), obj->GetVelocity(), tFirst, tLast);
}

void Bill::SetDirection(bool dir)
{
	Enemy::SetDirection(dir);
	if (auto* fullCap = dynamic_cast<BoundingCapsule<SFCapsule>*>(m_volume.get()))
	{
		auto which = dir ? HalfCapsule<SFCapsule>::Which::End
			: HalfCapsule<SFCapsule>::Which::Start;
		m_halfCap->Reset(fullCap, which);
		m_halfCap->Update(GetPosition());
	}
}

void Bill::Render(IRenderer* renderer)
{
	ENSURE_VALID(m_drawable);
	ENSURE_VALID(m_halfCap);

	m_drawable->Render(renderer);
	m_halfCap->Render(renderer);
}

void Bill::UpdateBody()
{
	if (m_halfCap)
		m_halfCap->Update(GetPosition());
}

void Bill::Animate(float deltaTime)
{
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());

	SetPrevPosition(GetPosition());

	if (HasLifes())
	{
		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);

			DECL_GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

			colMgr->ProcessCollisions(this);
		}
	}
	else
	{
		if (GetYVelocity() < GameMode::m_marioMaxSpdY)
			IncrementYVelocity(GameConstants::Gravity);

		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);

		ENSURE_VALID(m_volume);
		DECL_GET_OR_RETURN(camera, gameMgr->GetCamera());

		camera->CheckVerticalBounds(m_volume.get());
	}

	UpdateBody();

	GameMode::CheckForHorizontalBounds(deltaTime, this);
}