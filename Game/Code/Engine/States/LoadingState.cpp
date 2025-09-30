#include "LoadingState.h"

#include <thread>
#include <Engine/Core/Constants.h>
#include "MainMenuState.h"

void LoadResources()
{
	auto gameMgr = GameManager::Get();
	/*gameMgr->InitPlayer();*/

	auto scene = gameMgr->GetScene();

	scene->AddEnemies();
	scene->AddObjects();
	scene->AddForeGroundObjects();

	GameConstants::GameIsReady = true;
}

LoadingState::LoadingState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"), m_titleMessage(TextConfig("Pong"))
{
}

void LoadingState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	TextConfig config;
	config.m_alignment = TextAlignment::Center;
	config.m_position = GameConstants::ScreenDim / 2.0f;
	config.m_charSize = 30;
	config.m_animType = TextAnimType::Flashing;
	config.m_colour = Colour::Black;

	InitFlashingText(&m_titleMessage, "Loading", true, config);

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
	{
		m_gameMgr->GetGameStateMgr()->PopState();
	}
}

void LoadingState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();
	if (renderer)
	{
		m_backgroundSpr.Render(renderer);

		m_titleMessage.Render(renderer);
	}
}