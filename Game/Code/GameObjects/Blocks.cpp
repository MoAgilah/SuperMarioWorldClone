#include "Blocks.h"

#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>

QuestionBlock::QuestionBlock(const Vector2f& initPos)
	: Box(std::make_shared<SFAnimatedSprite>("QBlock", 2,4, GameConstants::FPS, false, 0.35f), std::make_shared<BoundingBox<SFRect>>(Vector2f(16, 16), Vector2f()), initPos)
{
	m_volume->Update(initPos);
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		spr->SetFrames({ 1, 4 });
		spr->ChangeAnim(QBlockAnims::ROTATE);
	}
}

void QuestionBlock::Update(float deltaTime)
{
	if (GetActive())
	{
		auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
		if (!spr)
			return;

		spr->Update(deltaTime);

		if (GetJustHit())
		{
			if (spr->GetCurrentAnim() != QBlockAnims::DUD)
				spr->ChangeAnim(QBlockAnims::DUD);

			SetCanHit(false);
			SetJustHit(false);
		}
	}
}

void QuestionBlock::Reset()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
	{
		if (spr->GetCurrentAnim() != QBlockAnims::ROTATE)
			spr->ChangeAnim(QBlockAnims::ROTATE);
	}

	Box::Reset();
}

RotatingBlock::RotatingBlock(const Vector2f& initPos)
	: Box(std::make_shared<SFAnimatedSprite>("RBlock", 2, 4, GameConstants::FPS, false, 0.35f), std::make_shared<BoundingBox<SFRect>>(Vector2f(16, 16), Vector2f()), initPos)
{
	m_volume->Update(initPos);
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (spr)
		spr->SetFrames({ 1, 4 });
}

void RotatingBlock::Update(float deltaTime)
{
	if (GetActive())
	{
		auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
		if (!spr)
			return;

		if (GetJustSmashed())
		{
			if (spr->GetTexID() != "BreakingBox")
			{
				spr->SetTexture("BreakingBox");
				spr->SetFrameData(2, 9, { 9, 9 });
				spr->SetScale(Vector2f(1, 1));

				if (spr->GetCurrentAnim() != ShatterAnims::SCATTER)
					spr->ChangeAnim(ShatterAnims::SCATTER);

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
					if (spr->GetCurrentAnim() != RBlockAnims::WAIT)
						spr->ChangeAnim(RBlockAnims::WAIT);

					SetCanHit(true);
				}
			}

			if (GetJustHit())
			{
				if (spr->GetCurrentAnim() != RBlockAnims::SPIN)
					spr->ChangeAnim(RBlockAnims::SPIN);

				SetCanHit(false);
				SetJustHit(false);
			}
		}
	}
}

void RotatingBlock::Reset()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;

	if (spr->GetTexID() != "RotatingBlock")
	{
		spr->SetTexture("RotatingBlock");
		spr->SetFrameData(2, 4, { 1, 4 });
	}

	if (spr->GetCurrentAnim() != RBlockAnims::WAIT)
		spr->ChangeAnim(RBlockAnims::WAIT);

	Box::Reset();
}

void RotatingBlock::Scatter()
{
	auto spr = dynamic_cast<SFAnimatedSprite*>(m_drawable.get());
	if (!spr)
		return;

	spr->Move(0, GameConstants::ObjectSpeed);

	if (GetPosition().y > 600.f + (float)spr->GetFrameSize().y)
	{
		SetActive(false);
	}

	if (spr->PlayedOnce())
	{
		if (spr->GetCurrentAnim() != ShatterAnims::DROP)
			spr->ChangeAnim(ShatterAnims::DROP);

		spr->SetShouldLoop(true);
	}
}