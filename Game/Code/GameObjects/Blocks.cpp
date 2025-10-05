#include "Blocks.h"

#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

QuestionBlock::QuestionBlock(const Vector2f& initPos)
	: Box(std::make_shared<SFAnimatedSprite>("QBlock", 2,4, GameConstants::FPS, false, 0.35f), std::make_shared<BoundingBox<SFRect>>(Vector2f(16, 16)), initPos)
{
	ENSURE_VALID(m_volume);
	m_volume->Update(initPos);

	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({ 1, 4 });
	spr->ChangeAnim(QBlockAnims::ROTATE);
}

void QuestionBlock::Update(float deltaTime)
{
	if (GetActive())
	{
		GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

		spr->Update(deltaTime);

		if (GetJustHit())
		{
			spr->EnsureAnim(QBlockAnims::DUD);

			SetCanHit(false);
			SetJustHit(false);
		}
	}
}

void QuestionBlock::Reset()
{
	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->EnsureAnim(QBlockAnims::ROTATE);

	Box::Reset();
}

RotatingBlock::RotatingBlock(const Vector2f& initPos)
	: Box(std::make_shared<SFAnimatedSprite>("RBlock", 2, 4, GameConstants::FPS, false, 0.35f), std::make_shared<BoundingBox<SFRect>>(Vector2f(16, 16), Vector2f()), initPos)
{
	ENSURE_VALID(m_volume);
	m_volume->Update(initPos);

	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->SetFrames({ 1, 4 });
}

void RotatingBlock::Update(float deltaTime)
{
	if (GetActive())
	{
		GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

		if (GetJustSmashed())
		{
			if (spr->GetTexID() != "BreakingBox")
			{
				spr->SetTexture("BreakingBox");
				spr->SetFrameData(2, 9, { 9, 9 });
				spr->SetScale(Vector2f(1, 1));

				spr->EnsureAnim(ShatterAnims::SCATTER);

				spr->SetShouldLoop(false);
			}

			spr->Update(deltaTime);
			Scatter();
		}
		else
		{
			if (!GetCanHit())
			{
				spr->Update(deltaTime);
				if (spr->PlayedNumTimes(2))
				{
					spr->EnsureAnim(RBlockAnims::WAIT);

					SetCanHit(true);
				}
			}

			if (GetJustHit())
			{
				spr->EnsureAnim(RBlockAnims::SPIN);

				SetCanHit(false);
				SetJustHit(false);
			}
		}
	}
}

void RotatingBlock::Reset()
{
	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	if (spr->GetTexID() != "RotatingBlock")
	{
		spr->SetTexture("RotatingBlock");
		spr->SetFrameData(2, 4, { 1, 4 });
	}

	spr->EnsureAnim(RBlockAnims::WAIT);

	Box::Reset();
}

void RotatingBlock::Scatter()
{
	GET_OR_RETURN(spr, GetAnimatedSprite(m_drawable.get()));

	spr->OffsetPosition(Vector2f(0, GameConstants::ObjectSpeed));

	if (GetPosition().y > 600.f + (float)spr->GetFrameSize().y)
	{
		SetActive(false);
	}

	if (spr->PlayedOnce())
	{
		spr->EnsureAnim(ShatterAnims::DROP);

		spr->SetShouldLoop(true);
	}
}