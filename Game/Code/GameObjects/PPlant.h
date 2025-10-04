#pragma once

#include <GameObjects/Enemy.h>

enum PPlantAnims { SINK, BITING };

class PPlant : public Enemy
{
public:
	explicit PPlant(const Vector2f& initPos);
	~PPlant() final = default;

	void Reset() final;

	void SetDirection(bool dir) override;

	void Die() final;

private:

	void Animate(float deltaTime) final;
};