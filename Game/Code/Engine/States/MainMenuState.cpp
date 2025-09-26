#include "MainMenuState.h"

#include <Engine/Core/Constants.h>
#include "MainState.h"

enum MenuPosition { Automation, Manual };

void MainMenuActionFunc(int menuPosition)
{
	switch (menuPosition)
	{
	case MenuPosition::Automation:
		//GameConstants::Automated = true;
		break;
	case MenuPosition::Manual:
		//GameConstants::Automated = false;
		break;
	}

	//GameManager::Get()->GetGameStateMgr()->ChangeState(new MainState(GameManager::Get()));
}

MainMenuState::MainMenuState(GameManager* gameMgr)
	: IGameState(gameMgr),
	m_backgroundSpr("Title"),
	m_menu(Vector2f(/*menuSize*/), 2.f, Vector2f(/*dimensions*/), MenuPositionData(MenuPositionMode::Centered,GameConstants::ScreenDim/2.f))
	/*m_menu(&MainMenuActionFunc,
		"Start Simulation",
		30,
		20,
		sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f))*/
{
	/*m_menu.AddMenuItem("Play Manually");*/

	m_gameMgr = gameMgr;
}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());
}

void MainMenuState::Pause()
{}

void MainMenuState::Resume()
{}

void MainMenuState::ProcessInputs()
{}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void MainMenuState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();
	if (renderer)
	{
		m_backgroundSpr.Render(renderer);

		m_menu.Render(renderer);
	}
}
