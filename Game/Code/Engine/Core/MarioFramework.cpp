#include "MarioFramework.h"

#include "../Collisions/MarioCollisionManager.h"
#include "../States/DebugState.h"
#include "../States/TitleState.h"
#include <Collisions/SFGrid.h>
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>

MarioFramework::MarioFramework()
{
	GameConstants::Gravity = 0.1875f;
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

	GET_OR_RETURN(renderer, m_gameMgr.GetRenderer());
	renderer->Initialise(GameConstants::ScreenDim, GameConstants::WindowTitle);

	GameConstants::TileFilePaths = "../Game/Resources/TileTypes.txt";

	m_gameMgr.SetICollisionManager(std::make_shared<MarioCollisionManager>(std::make_shared<SFGrid>(15, 313, "Arial", GameConstants::TileFilePaths)));
	ENSURE_VALID(m_gameMgr.GetCollisionMgr());

	m_gameMgr.GetGameStateMgr()->ChangeState(new TitleState(&m_gameMgr));
}

void MarioFramework::Shutdown()
{
}