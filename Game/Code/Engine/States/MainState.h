#pragma once

#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

class MainState : public IGameState
{
public:
	explicit MainState(GameManager* gameMgr);
	~MainState() override = default;

	std::string_view GetStateName() const { return "Main"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	bool m_ready = false;
	bool m_isPaused = false;
};