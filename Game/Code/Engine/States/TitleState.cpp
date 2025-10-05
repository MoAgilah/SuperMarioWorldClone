#include "TitleState.h"

#include "MainMenuState.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>

TitleState::TitleState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"), m_titleMessage(TextConfig("Pong"))
{

}

void TitleState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	TextConfig config;
	config.m_alignment = TextAlignment::Center;
	config.m_position = GameConstants::ScreenDim / 2.0f;
	config.m_charSize = 30;
	config.m_animType = TextAnimType::Flashing;
	config.m_colour = Colour::Black;

	InitFlashingText(&m_titleMessage, "Press Any Key To Start", true, config);
}

void TitleState::Pause()
{
}

void TitleState::Resume()
{
}

void TitleState::ProcessInputs()
{
	ENSURE_VALID(m_gameMgr);
	GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->IsAnyKeyPressed())
		m_gameMgr->GetGameStateMgr()->ChangeState(new MainMenuState(m_gameMgr));
}

void TitleState::Update(float deltaTime)
{
	ProcessInputs();

	m_titleMessage.Update(deltaTime);
}

void TitleState::Render()
{
	ENSURE_VALID(m_gameMgr);
	GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_titleMessage.Render(renderer);
}