#pragma once

enum class GameType
{
	NotSet = -1,
	Manual,
	Automation
};

class GameMode
{
public:
	static GameType m_gameType;
	static float m_mariosMaxSpdX;
	static float m_marioMaxSpdY;
};