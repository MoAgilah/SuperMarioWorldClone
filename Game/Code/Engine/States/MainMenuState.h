#pragma once

#include <Drawables/SFSprite.h>
#include <UI/SFMenu.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

class MainMenuState : public IGameState
{
public:
	explicit MainMenuState(GameManager* gameMgr);
	~MainMenuState() override = default;

	std::string_view GetStateName() const { return "MainMenu"; }

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