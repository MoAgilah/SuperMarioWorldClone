#include "TitleState.h"

#include "MainMenuState.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>
#include <Utilities/Vector.h>

TitleState::TitleState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"), m_titleSpr("Clone"),
	m_titleMessage({"SMW",  30, GameConstants::ScaleScreenDim(0.5f, 0.65f), TextAnimType::Flashing, Colour::Black, TextAlignment::Center})
{
}

void TitleState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());
	m_backgroundSpr.SetPosition(Vector2f());

	m_titleSpr.SetPosition(GameConstants::ScaleScreenDim(0.5f, 0.55f));

	m_titleMessage.InitFlashingText("Press Any Key To Start");
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
	DECL_GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

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
	DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_titleSpr.Render(renderer);
	m_titleMessage.Render(renderer);
}