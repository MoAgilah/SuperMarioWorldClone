#include "MainMenuState.h"

#include "MainState.h"
#include "../../Utilities/GameMode.h"
#include <Drawables/SFText.h>
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>

enum MenuPosition { Automation, Manual };

MainMenuState::MainMenuState(GameManager* gameMgr)
	: IGameState(gameMgr),
	m_backgroundSpr("Title"),
	m_menu(Vector2f({ GameConstants::ScreenDim.x * 0.8f, GameConstants::ScreenDim.y * 0.4f }), 2.f, {1, 2}, MenuPositionData(MenuPositionMode::Centered, GameConstants::ScreenDim / 2.f))
{}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	auto cellSize = m_menu.GetCellSize();

	TextConfig config;
	config.m_fontName = "SuperMarioWorld";
	config.m_charSize = static_cast<int>(cellSize.y * 0.6f);
	config.m_animType = TextAnimType::Flashing;
	config.m_alignment = TextAlignment::Center;
	config.m_colour = Colour::Black;

	auto cell = m_menu.GetCell({ 1,1 });

	ENSURE_VALID(cell);
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Automation");
		}
		cell->SetMenuSlotNumber(0);
	}

	cell = m_menu.GetCell({ 1,2 });

	ENSURE_VALID(cell);
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Manual");
		}
		cell->SetMenuSlotNumber(1);
	}

	m_menu.SetActiveCells();

	auto menuNav = m_menu.GetMenuNav();
	ENSURE_VALID(menuNav);

	menuNav->SetCursorRange({ 0,1 });
	menuNav->SetCurrCursorPos(0);
}

void MainMenuState::Pause()
{}

void MainMenuState::Resume()
{}

void MainMenuState::ProcessInputs()
{
	ENSURE_VALID(m_gameMgr);
	GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->GetKeyState(static_cast<int>(KeyCode::Enter)))
	{
		GET_OR_RETURN(menuNav, m_menu.GetMenuNav());
		switch (menuNav->GetCurrCursorPos())
		{
		case MenuPosition::Automation:
			GameMode::m_gameType = GameType::Automation;
			break;
		case MenuPosition::Manual:
			GameMode::m_gameType = GameType::Manual;
			break;
		}

		m_gameMgr->GetGameStateMgr()->ChangeState(new MainState(GameManager::Get()));
	}
}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void MainMenuState::Render()
{
	GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_menu.Render(renderer);
}
