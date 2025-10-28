#pragma once

#include <Utilities/Vector.h>
#include <memory>

enum class GameType
{
	NotSet = -1,
	Manual,
	Automation
};

class Player;
class IDynamicGameObject;

class GameMode
{
public:
	static void InitPlayer();
	static void ToTitle();
	static void CheckForHorizontalBounds(float deltaTime, IDynamicGameObject* obj);

	static std::shared_ptr<Player> s_player;
	static GameType m_gameType;
	static float m_leftMost;
	static float m_rightMost;
	static float m_mariosMaxSpdX;
	static float m_marioMaxSpdY;
	static Vector2f m_SnesResolution;

	static Player* GetPlayer() { return s_player.get(); }
};