#pragma once

#include <Engine/Core/Timer.h>
#include <Engine/Interface/Scene/IObjectState.h>
#include <Utilities/MovementController.h>
#include <memory>

class Player;
class SFAnimatedSprite;

class PlayerState : public IObjectState
{
public:
	PlayerState(Player* ply);
	~PlayerState();

	void Pause() override;
	void Resume() override;

	SFAnimatedSprite* GetAnimSpr();
	Player* GetPlayer();

protected:

	bool m_initialisedCtrl = false;
	static MovementController m_movementCtrl;
};

class GroundedState : public PlayerState
{
public:
	GroundedState(Player* ply);
	~GroundedState() = default;

	std::string_view GetStateName() const override { return "Grounded"; }

	void Resume() override;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;
};

class SlopedState : public PlayerState
{
public:
	SlopedState(Player* ply);
	~SlopedState() = default;

	std::string_view GetStateName() const override { return "Sloped"; }

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;
};

class AirborneState : public PlayerState
{
public:
	AirborneState(Player* ply);
	~AirborneState() = default;

	std::string_view GetStateName() const override { return "Airborne"; }

	void Resume() override;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateAnimation() override;
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
};