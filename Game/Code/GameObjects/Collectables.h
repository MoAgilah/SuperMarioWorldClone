#pragma once

#include <Engine/Core/Timer.h>
#include <GameObjects/Collectables.h>

class Player;

class Coin : public StaticCollectable
{
public:
	explicit Coin(const Vector2f& initPos);
	~Coin() final = default;

	void Update(float deltaTime) final;

	void Collect(GameObject* obj) final;
};

class YCoin : public StaticCollectable
{
public:
	explicit YCoin(const Vector2f& initPos);
	~YCoin() final = default;

	void Update(float deltaTime) final;

	void Reset() final;

	void Collect(GameObject* obj) final;

private:

	static int s_collected;
};

class CheckPoint : public StaticCollectable
{
public:
	explicit CheckPoint(const Vector2f& initPos);
	~CheckPoint() final = default;

	void Update(float deltaTime) final;

	void Collect(GameObject* obj) final;
};

class Mushroom : public DynamicCollectable
{
public:
	explicit Mushroom(const Vector2f& initPos);
	~Mushroom() final = default;

	void Update(float deltaTime) final;

	void Collect(GameObject* obj) final;
};

class Goal : public DynamicCollectable
{
public:
	explicit Goal(const Vector2f& initPos);
	~Goal() final = default;

	void Update(float deltaTime) final;

	void Collect(GameObject* obj) final;

private:

	const float c_maxTravelTime = 2.15f;
	Timer m_airTimer;
};