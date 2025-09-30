#pragma once

#include <GameObjects/GameObject.h>
#include <string>

class Pipe : public GameObject
{
public:
	Pipe(const std::string& texID, const Vector2f& pos);

	void Update(float deltaTime) override;

	void OnCollisionEnter(IGameObject* obj) override;
	void OnCollisionStay(IGameObject* obj) override;
	void OnCollisionExit(IGameObject* obj) override;
	void ResolveCollisions(float time, const Vector2f& separationVector, float relativeHitPosition) override;
};