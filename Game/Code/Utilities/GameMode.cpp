#include "GameMode.h"

#include "../Engine/Collisions/MarioCollisionManager.h"
#include "../Engine/States/TitleState.h"
#include "../GameObjects/Player.h"
#include <Collisions/SFGrid.h>
#include <GameObjects/GameObject.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Collisions/ITile.h>
#include <Utilities/Utils.h>

std::shared_ptr<Player> GameMode::s_player = nullptr;
GameType GameMode::m_gameType = GameType::Manual;
float GameMode::m_leftMost = 15.6f;
float GameMode::m_rightMost = 11400.f;
float GameMode::m_mariosMaxSpdX = 0.f;
float GameMode::m_marioMaxSpdY = 0.f;
Vector2f GameMode::m_SnesResolution = Vector2f(256.f, 224.f);

void GameMode::InitPlayer()
{
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(colMgr,gameMgr->GetCollisionMgr());
	DECL_GET_OR_RETURN(cam, gameMgr->GetCamera());
	DECL_GET_OR_RETURN(tile, colMgr->GetTile(1, 10));

	switch (m_gameType)
	{
	case GameType::Manual:
		s_player = std::make_shared<Player>(tile->GetPosition());
		cam->SetObjectToFollow(s_player);
		break;
	case GameType::Automation:
		break;
	}
}

void GameMode::ToTitle()
{
	auto gameMgr = GameManager::Get();
	if (!gameMgr)
		return;

	auto stateMgr = gameMgr->GetGameStateMgr();
	if (!stateMgr)
		return;

	gameMgr->SetICollisionManager(std::make_shared<MarioCollisionManager>(std::make_shared<SFGrid>(15, 313, "Arial", GameConstants::TileFilePaths)));
	gameMgr->SetScene(nullptr);
	GameConstants::GameIsReady = false;

	stateMgr->ClearStates();
	stateMgr->ChangeState(new TitleState(gameMgr));

	if (s_player)
		s_player = nullptr;
}

void GameMode::CheckForHorizontalBounds(float deltaTime, IDynamicGameObject* obj)
{
	ENSURE_VALID(obj);
	DECL_GET_OR_RETURN(volume, obj->GetVolume());

	if (volume->GetPoint(Side::Left).x <= m_leftMost)
	{
		obj->Move(-obj->GetVelocity().x * GameConstants::FPS * deltaTime, 0);

		auto ply = dynamic_cast<Player*>(obj);
		if (!ply)
			obj->SetDirection(!obj->GetDirection());
	}
}