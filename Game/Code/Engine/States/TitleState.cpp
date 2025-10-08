#include "TitleState.h"

#include "MainMenuState.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>
#include <Utilities/Vector.h>

TitleState::TitleState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"), m_titleSpr("Clone"),
	m_titleMessage({"supermarioworld",  30, GameConstants::ScreenDim / 2.0f, TextAnimType::Flashing})
{

}

void TitleState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	m_titleSpr.SetPosition(Vector2f(GameConstants::ScreenDim.x * 0.5f, GameConstants::ScreenDim.y * 0.55f));

	m_titleSpr.SetScale(GameConstants::Scale);
	m_titleSpr.SetOrigin(Vector2f(m_titleSpr.GetSize().x / 2, m_titleSpr.GetSize().y / 2));

	TextConfig config;
	config.m_fontName = "Arial";
	config.m_alignment = TextAlignment::Center;
	config.m_position = Vector2f(GameConstants::ScreenDim.x * 0.5f, GameConstants::ScreenDim.y * 0.65f);
	config.m_charSize = 30;
	config.m_animType = TextAnimType::Flashing;
	config.m_colour = Colour::Black;

	InitFlashingText(&m_titleMessage, "Press Any Key To Start", true, config);

	m_gameMgr->GetCamera()->Reset(m_gameMgr->GetRenderer());
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
	m_titleSpr.Render(renderer);
	m_titleMessage.Render(renderer);
}