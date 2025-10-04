#include "Chuck.h"

#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>

Chuck::Chuck(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Chuck",5,7,GameConstants::FPS, false,0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(24.f,25.f))), m_waitTimer(0.5f)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	m_volume->Update(GetPosition());
	SetSpeedX(GameMode::m_mariosMaxSpdX * 0.60f);
	SetSpeedY(GameMode::m_marioMaxSpdY);

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

namespace {
    // Upward takeoff speed in px/frame (SMW-ish gravity: 0.1875 px/f^2)
    constexpr float kChuckJumpVy = -3.5f; // ~2 tiles apex; try -3.0f (1.5 tiles) or -3.9f (2.5 tiles)
}

void Chuck::Animate(float dt)
{
    auto* spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
    if (!spr)
        return;

    spr->Update(dt);

    if (GetOnGround())
    {
        m_waitTimer.Update(dt);
        if (m_waitTimer.CheckEnd())
        {
            spr->EnsureAnim(ChuckAnims::BOUNCE);
            SetAirTime(0.75f);
            SetYVelocity(GameMode::m_marioMaxSpdY);
            SetAirbourne(true);
        }
        else
        {
            SetYVelocity(0);
        }
    }
    else
    {
        if (GetAirbourne())
        {
            if (GetAirTimer()->CheckEnd())
            {
                spr->EnsureAnim(ChuckAnims::CLAP);
                SetAirbourne(false);
                SetYVelocity(0);
            }
        }
        else
        {
            if (!GetOnGround())
            {
                if (GetYVelocity() < GameMode::m_marioMaxSpdY)
                    IncrementYVelocity(GameConstants::Gravity);
            }
        }
    }

    if (GetYVelocity() != 0.0f)
    {
        Move(0.0f, GetYVelocity() * GameConstants::FPS * dt);
        GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
    }
}