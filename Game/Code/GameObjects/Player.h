#pragma once

#include <array>
#include <string>

#include <Engine/Core/Constants.h>
#include <Engine/Core/Timer.h>
#include <Engine/Input/KeyCode.h>
#include <GameObjects/GameObject.h>
#include "../Engine/States/PlayerState.h"
#include <Engine/Core/GameStateMgr.h>

enum MarioAnims { IDLE, LOOKUP, JUMP, FALL, RUNJUMP, SKID, SLIDE, VICTORY, CROUCH, GROW, DIE, MOVING, RUNNING, SPINJUMP };

enum MarioBoxes { REGULAR, SUPER, CROUCHED, NUMBOXES };

class Timer;

namespace sf { class Shader; }

class SFAnimatedSprite;

enum Keys
{
	LEFT_KEY = KeyCode::Left,
	RIGHT_KEY = KeyCode::Right,
	UP_KEY = KeyCode::Up,
	DOWN_KEY = KeyCode::Down,
	RUN_KEY = KeyCode::S,
	JUMP_KEY = KeyCode::A,
	SJUMP_KEY = KeyCode::D,
	MAXKEYS
};

class CNeuralNet;

class Player : public DynamicGameObject
{
public:
	explicit Player(const Vector2f& pos);
	~Player() override = default;

	void Update(float deltaTime) final;
	void Render(IRenderer* renderer) final;
	void Reset() override;

	bool GetIsSuper() const { return m_super; }
	void SetIsSuper(bool super);

	bool GetGoalHit() const { return m_goalHit; }
	void SetGoalHit(bool goal) { m_goalHit = goal; }

	bool GetIsAlive() const { return m_alive; }
	void SetIsAlive(bool val, float airtime = 0.1f);

	void IncreaseCoins(int num) { m_coinTotal = +num; }

	void SetSpawnLoc(Vector2f loc = Vector2f(0, 0));

	bool GetCantJump() const { return m_cantjump; }
	void SetCantJump(bool val) { m_cantjump = val; }

	bool GetCantSpinJump() const { return m_cantSpinJump; }
	void SetCantSpinJump(bool val) { m_cantSpinJump = val; }

	bool GetIsCrouched() const { return m_crouched; }
	void SetIsCrouched(bool crouched);

	void ForceFall();
	void Bounce();

	bool GetIfInvulnerable() const { return !m_invulTimer.CheckEnd(); }
	void SetInvulnerability();

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air);

	Timer* GetAirTimer() { return &m_airTimer; }

	SFAnimatedSprite* GetAnimatedSprite();

private:

	void ProcessInput();
	virtual void Input();

	bool m_super = false;
	bool m_crouched = false;
	bool m_alive = true;
	bool m_cantjump = false;
	bool m_cantSpinJump = false;
	bool m_goalHit = false;
	bool m_airbourne = false;
	int m_coinTotal = 0;
	float m_heightDiff = 11.25;
	Timer m_airTimer;
	Timer m_invulTimer;
	Vector2f m_spawnLoc;
	sf::Shader* m_fragShader;
	GameStateMgr<IObjectState> m_stateMgr;
	std::array<Vector2f, MarioBoxes::NUMBOXES> m_boxSizes{ Vector2f(9,16), Vector2f(9,25), Vector2f(14,11) };
};