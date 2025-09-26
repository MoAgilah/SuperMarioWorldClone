#include "MainState.h"

#include <Engine/Input/KeyCode.h>
#include <Engine/Core/Constants.h>
#include "LoadingState.h"
#include "PauseMenuState.h"

MainState::MainState(GameManager* gameMgr)
	: IGameState(gameMgr)
{

}

void MainState::Initialise()
{
	//m_gameMgr->GetGameStateMgr()->PushState(new LoadingState(m_gameMgr));
}

void MainState::Pause()
{
	m_ready = false;
}

void MainState::Resume()
{
	m_ready = true;
	m_isPaused = false;
}

void MainState::ProcessInputs()
{
	/*if (sf::Keyboard::isKeyPressed(KeyCode::Space))
	{
		if (!m_isPaused)
		{
			m_gameMgr->GetCamera().Reset(m_gameMgr->GetRenderWindow());
			m_gameMgr->GetGameStateMgr()->PushState(new PauseMenuState(m_gameMgr));
			m_isPaused = true;
		}
	}*/
}

void MainState::Update(float deltaTime)
{
	if (m_ready)
	{
		ProcessInputs();

		m_gameMgr->GetTimer().Update(deltaTime);

		m_gameMgr->CheckInView();

		m_gameMgr->GetScene()->Update(deltaTime);
		/*m_gameMgr->GetPlayer()->Update(deltaTime);*/
	}
}

void MainState::Render()
{
	if (m_ready)
	{
		/*m_gameMgr->GetCamera().Reset(window);*/

		auto renderer = m_gameMgr->GetRenderer();
		if (renderer)
		{
			m_gameMgr->GetScene()->Render(renderer);
			/*m_gameMgr->GetPlayer()->Render(window);*/

			m_gameMgr->GetCollisionMgr()->Render(renderer);
		}
	}
}