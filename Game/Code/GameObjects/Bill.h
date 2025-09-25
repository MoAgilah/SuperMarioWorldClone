#pragma once

#include "Player.h"
#include <GameObjects/Enemy.h>
#include <Utilities/Vector.h>

struct HalfCapsule
{
//	sf::CircleShape circle;
//	AABB box;
//
//	bool Intersects(AABB* box);
//
//private:
//
//	bool CircleToAABB(AABB* box);
};

class Bill : public Enemy
{
public:
	Bill(bool dir, const Vector2f& initPos);
	~Bill() final = default;

	void Render(IRenderer* renderer) final;
	bool Intersects(IGameObject* obj) override;

	bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) override;
	//bool IsPlayerAbove(Player* ply) final;

	void Die() final { SetTimeLeftActive(2.f); }

	bool GetAirbourne() const = delete;
	void SetAirbourne(bool air) = delete;
	float GetAirTime() const = delete;
	void SetAirTime(float val) = delete;
	void IncAirTime(float val) = delete;

private:

	void UpdateBody();
	void Animate(float deltaTime) final;

	HalfCapsule m_halfCap;
};