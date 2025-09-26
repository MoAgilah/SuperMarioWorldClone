#include "PauseMenuState.h"

#include <Engine/Core/Constants.h>
#include "TitleState.h"

enum MenuPosition { Resume, ToTitle, Quit };

void PauseMenuActionFunc(int menuPosition)
{
	auto gameMgr = GameManager::Get();

	switch (menuPosition)
	{
	case MenuPosition::Resume:
		gameMgr->GetGameStateMgr()->PopState();
		break;
	case MenuPosition::ToTitle:
		/*gameMgr->Reinitialise();
		gameMgr->GetGameStateMgr()->ClearStates();
		GameConstants::GameIsReady = false;
		gameMgr->GetGameStateMgr()->ChangeState(new TitleState(gameMgr));*/
		break;
	case MenuPosition::Quit:
		/*gameMgr->GetRenderWindow().close();*/
		break;
	}
}

PauseMenuState::PauseMenuState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"),
	m_menu(Vector2f(/*menuSize*/), 2.f, Vector2f(/*dimensions*/), MenuPositionData(MenuPositionMode::Centered, GameConstants::ScreenDim / 2.f))
	/*m_menu(&PauseMenuActionFunc,
		"Resume",
		30,
		20,
		sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f))*/
{
	/*m_menu.AddMenuItem("Return To Title Screen");
	m_menu.AddMenuItem("Quit");*/

	m_gameMgr = gameMgr;
}

void PauseMenuState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());
}

void PauseMenuState::Pause()
{
}

void PauseMenuState::Resume()
{}

void PauseMenuState::ProcessInputs()
{}

void PauseMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void PauseMenuState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();
	if (renderer)
	{
		m_backgroundSpr.Render(renderer);

		m_menu.Render(renderer);
	}
}
