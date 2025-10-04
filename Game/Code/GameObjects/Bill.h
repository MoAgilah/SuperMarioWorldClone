#pragma once

#include "Player.h"
#include <GameObjects/Enemy.h>
#include <Utilities/Vector.h>
#include <memory>

template <typename TCapsule>
struct HalfCapsule;

class SFCapsule;

class Bill : public Enemy
{
public:
	Bill(bool dir, const Vector2f& initPos);
	~Bill() final = default;

	void Render(IRenderer* renderer) final;
	bool Intersects(IGameObject* obj) override;

	bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) override;

	void Die() final { SetTimeLeftActive(2.f); }

	void SetDirection(bool dir);

	bool GetAirbourne() const = delete;
	void SetAirbourne(bool air) = delete;
	float GetAirTime() const = delete;
	void SetAirTime(float val) = delete;
	void IncAirTime(float val) = delete;

private:

	void UpdateBody();
	void Animate(float deltaTime) final;

	std::shared_ptr<HalfCapsule<SFCapsule>> m_halfCap;
};
