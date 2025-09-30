#include "Pipes.h"

#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Engine/Collisions/BoundingBox.h>

Pipe::Pipe(const std::string& texID, const Vector2f& pos)
	: GameObject(std::make_shared<SFSprite>(texID),std::make_shared<BoundingBox<SFRect>>())
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());

	auto box = dynamic_cast<BoundingBox<SFRect>*>(m_volume.get());
	if (box)
		box->Reset(m_drawable->GetSize());

	m_volume->Update(GetPosition());
}

void Pipe::Update(float deltaTime)
{

}

void Pipe::OnCollisionEnter(IGameObject* obj)
{

}

void Pipe::OnCollisionStay(IGameObject* obj)
{

}

void Pipe::OnCollisionExit(IGameObject* obj)
{

}

void Pipe::ResolveCollisions(float time, const Vector2f& separationVector, float relativeHitPosition)
{

}