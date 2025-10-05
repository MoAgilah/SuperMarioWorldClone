#include "Chuck.h"

#include "../Utilities/GameMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

Chuck::Chuck(bool dir, const Vector2f& initPos)
	: Enemy(std::make_shared<SFAnimatedSprite>("Chuck",5,7,GameConstants::FPS, false,0.5f),
		std::make_shared<BoundingBox<SFRect>>(Vector2f(24.f,25.f))), m_waitTimer(0.5f)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
    SetSpeedX(GameMode::m_mariosMaxSpdX * 0.60f);
    SetSpeedY(GameMode::m_marioMaxSpdY);

    ENSURE_VALID(m_volume);
	m_volume->Update(GetPosition());

    GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

    spr->SetFrames({ 3, 1, 1, 7, 3 });
}

void Chuck::Reset()
{
    GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(ChuckAnims::BOUNCE);

	Enemy::Reset();
}

void Chuck::Die()
{
    GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(ChuckAnims::WHIPLASH);

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

        GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

		spr->EnsureAnim(ChuckAnims::HIT);
	}
}

void Chuck::Animate(float dt)
{
    ENSURE_VALID(GetAirTimer());
    GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

    spr->Update(dt);

    if (HasLifes())
    {
        SetPrevPosition(GetPosition());

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

        GET_OR_RETURN(gameMgr, GameManager::Get());
        GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

        if (GetYVelocity() != 0.0f)
        {
            Move(0.0f, GetYVelocity() * GameConstants::FPS * dt);
            colMgr->ProcessCollisions(this);
        }
    }
}