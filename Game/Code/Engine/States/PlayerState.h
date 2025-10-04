#pragma once

#include <Engine/Core/Timer.h>
#include <Engine/Interface/Scene/IObjectState.h>
#include <Utilities/MovementController.h>
#include <memory>

struct Speed
{
	static constexpr float Low = 1.f;
	static constexpr float Med = 2.f;
	static constexpr float High = 3.f;
};

class Player;
class SFAnimatedSprite;

class PlayerState : public IObjectState
{
public:
	PlayerState(Player* ply);
	~PlayerState();

	void Pause() override;
	void Resume() override;

	void Update(float deltaTime) override;

	SFAnimatedSprite* GetAnimSpr();
	Player* GetPlayer();

protected:

	void MoveLeft(Player* ply);
	void MoveRight(Player* ply);
	void MoveUp(Player* ply);
	void MoveDown(Player* ply);

	void InitiateJump(Player* ply);
	void InititateSpinJump(Player* ply);

	bool m_initialisedCtrl = false;
	float m_walkCap = 0;
	float m_runCap = 0;
	float m_sprintCap = 0;
	static MovementController m_movementCtrl;
};

class LateralState : public PlayerState
{
public:
	LateralState(Player* ply);
	~LateralState() = default;

	std::string_view GetStateName() const override { return "Grounded"; }

	void Resume() override;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;

	bool m_runKeyHeld = false;
};

class CrouchedState : public PlayerState
{
public:
	CrouchedState(Player* ply);
	~CrouchedState() = default;

	std::string_view GetStateName() const override { return "Crouched"; }

	void Resume() override;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;
};

class InclinedState : public PlayerState
{
public:
	InclinedState(Player* ply);
	~InclinedState() = default;

	std::string_view GetStateName() const override { return "Sloped"; }

	void Resume() override;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;

	bool m_sliding = true;
};

class VerticalState : public PlayerState
{
public:
	VerticalState(Player* ply, bool spinJump = false);
	~VerticalState() = default;

	std::string_view GetStateName() const override { return "Airborne"; }

	void Resume() override;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;

	Timer* m_airTimer;
	bool m_spinJump = false;
};

class DieingState : public PlayerState
{
public:
	DieingState(Player* ply);
	~DieingState() = default;

	std::string_view GetStateName() const override { return "Dieing"; }

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;

	Timer* m_airTimer;
};