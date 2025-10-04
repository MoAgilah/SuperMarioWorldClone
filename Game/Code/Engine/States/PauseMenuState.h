#pragma once

#include <Drawables/SFSprite.h>
#include <UI/SFMenu.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

class PauseMenuState : public IGameState
{
public:
	explicit PauseMenuState(GameManager* gameMgr);
	~PauseMenuState() override = default;

	std::string_view GetStateName() const { return "Pause"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	SFMenu m_menu;
	SFSprite m_backgroundSpr;
};