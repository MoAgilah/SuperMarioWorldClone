#include "MainState.h"

#include "LoadingState.h"
#include "PauseMenuState.h"
#include "../../GameObjects/Player.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <Engine/Input/KeyCode.h>
#include <Utilities/Utils.h>

MainState::MainState(GameManager* gameMgr)
	: IGameState(gameMgr)
{

}

void MainState::Initialise()
{
	ENSURE_VALID(m_gameMgr);
	m_gameMgr->GetGameStateMgr()->PushState(new LoadingState(m_gameMgr));
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
	ENSURE_VALID(m_gameMgr);
	GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->GetKeyState(static_cast<int>(KeyCode::Space)))
	{
		if (!m_isPaused)
		{
			m_gameMgr->GetGameStateMgr()->PushState(new PauseMenuState(m_gameMgr));
			m_isPaused = true;
		}
	}
}

void MainState::Update(float deltaTime)
{
	ENSURE_VALID(m_gameMgr);

	if (m_ready)
	{
		GET_OR_RETURN(ply, GameMode::GetPlayer());

		ProcessInputs();

		m_gameMgr->GetTimer().Update(deltaTime);

		m_gameMgr->CheckInView();

		m_gameMgr->GetScene()->Update(deltaTime);

		ply->Update(deltaTime);
	}
}

void MainState::Render()
{
	ENSURE_VALID(m_gameMgr);

	if (m_ready)
	{
		GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());
		GET_OR_RETURN(ply, GameMode::GetPlayer());

		m_gameMgr->GetCamera()->Reset(renderer);

		m_gameMgr->GetScene()->Render(renderer);

		ply->Render(renderer);

		GET_OR_RETURN(colMgr, m_gameMgr->GetCollisionMgr());

		colMgr->Render(renderer);
	}
}