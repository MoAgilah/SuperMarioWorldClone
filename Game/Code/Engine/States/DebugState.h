#pragma once

#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

class DebugState : public IGameState
{
public:
	explicit DebugState(GameManager* gameMgr);
	~DebugState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

};