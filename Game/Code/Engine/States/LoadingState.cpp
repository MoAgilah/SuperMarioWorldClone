#include "LoadingState.h"

#include "MainMenuState.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>
#include <thread>

void LoadResources()
{
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());

	GameMode::InitPlayer();

	DECL_GET_OR_RETURN(ply, GameMode::GetPlayer());

	DECL_GET_OR_RETURN(scene, gameMgr->GetScene());

	GameConstants::GameIsReady = scene->Initialise();
}

LoadingState::LoadingState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"),
	m_titleMessage({ "SMW",  30, GameConstants::ScaleScreenDim(0.5f, 0.65f), TextAnimType::Flashing, Colour::Black, TextAlignment::Center })
{
}

void LoadingState::Initialise()
{
	auto scl = GameConstants::ScreenDim.x / GameMode::m_SnesResolution.x;
	m_backgroundSpr.SetScale({ scl, scl });
	m_backgroundSpr.SetOrigin(Vector2f());

	m_titleMessage.SetText("Loading");

	std::thread t(LoadResources);
	t.detach();
}

void LoadingState::Pause()
{
}

void LoadingState::Resume()
{
}

void LoadingState::ProcessInputs()
{
}

void LoadingState::Update(float deltaTime)
{
	ProcessInputs();

	m_titleMessage.Update(deltaTime);

	if (GameConstants::GameIsReady)
		m_gameMgr->GetGameStateMgr()->PopState();
}

void LoadingState::Render()
{
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());
	m_backgroundSpr.Render(renderer);
	m_titleMessage.Render(renderer);
}