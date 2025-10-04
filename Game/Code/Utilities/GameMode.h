#pragma once

#include <memory>

enum class GameType
{
	NotSet = -1,
	Manual,
	Automation
};

class Player;

class GameMode
{
public:
	static void InitPlayer();

	static std::shared_ptr<Player> s_player;
	static GameType m_gameType;
	static float m_mariosMaxSpdX;
	static float m_marioMaxSpdY;

	static Player* GetPlayer() { return s_player.get(); }
};