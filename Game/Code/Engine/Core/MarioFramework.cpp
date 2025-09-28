#include "MarioFramework.h"

#include "../Collisions/MarioCollisionManager.h"
#include "../States/TitleState.h"
#include "../States/DebugState.h"
#include <Collisions/SFGrid.h>
#include <Engine/Core/Constants.h>

MarioFramework::MarioFramework()
{
	GameConstants::ObjectSpeed = 8.f;
	GameConstants::WindowTitle = "SFMLMario";
	GameConstants::WindowColour = Colour::Black;

	Initialise();
}

void MarioFramework::Initialise()
{
	m_gameMgr.GetFontMgr().AddFonts("../Game/Resources/Fonts/");
	m_gameMgr.GetShaderMgr().AddShaders("../Game/Resources/Shaders/");
	m_gameMgr.GetSoundMgr().AddSounds("../Game/Resources/Sounds/");
	m_gameMgr.GetSoundMgr().AddMusic("../Game/Resources/Music/");
	m_gameMgr.GetTextureMgr().AddTextures("../Game/Resources/Textures/");

	m_gameMgr.GetRenderer()->Initialise(GameConstants::ScreenDim, GameConstants::WindowTitle);

	GameConstants::TileFilePaths = "../Game/Resources/TileTypes.txt";

	m_gameMgr.SetICollisionManager(std::make_shared<MarioCollisionManager>(std::make_shared<SFGrid>(15, 313, "Arial", GameConstants::TileFilePaths)));

	m_gameMgr.GetGameStateMgr()->ChangeState(new TitleState(&m_gameMgr));
}

void MarioFramework::Shutdown()
{
}