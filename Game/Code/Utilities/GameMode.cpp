#include "GameMode.h"

#include "../GameObjects/Player.h"
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Collisions/ITile.h>

std::shared_ptr<Player> GameMode::s_player = nullptr;
GameType GameMode::m_gameType = GameType::Manual;
float GameMode::m_mariosMaxSpdX = 0;
float GameMode::m_marioMaxSpdY = 0;

void GameMode::InitPlayer()
{
	auto gameMgr = GameManager::Get();
	if (!gameMgr)
		return;

	auto colMgr = gameMgr->GetCollisionMgr();
	if (!colMgr)
		return;

	auto tile = colMgr->GetTile(2, 11);
	if (!tile)
		return;

	switch (m_gameType)
	{
	case GameType::Manual:
		s_player = std::make_shared<Player>(tile->GetPosition());
		break;
	case GameType::Automation:
		break;
	}
}
