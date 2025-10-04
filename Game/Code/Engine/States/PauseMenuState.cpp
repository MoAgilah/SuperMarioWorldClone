#include "PauseMenuState.h"

#include "TitleState.h"
#include <Engine/Core/Constants.h>

enum MenuPosition { Resume, ToTitle, Quit };

void PauseMenuActionFunc(int menuPosition)
{
	auto gameMgr = GameManager::Get();

	switch (menuPosition)
	{
	case MenuPosition::Resume:
		gameMgr->GetGameStateMgr()->PopState();
		break;
	case MenuPosition::ToTitle:
		/*gameMgr->Reinitialise();
		gameMgr->GetGameStateMgr()->ClearStates();
		GameConstants::GameIsReady = false;
		gameMgr->GetGameStateMgr()->ChangeState(new TitleState(gameMgr));*/
		break;
	case MenuPosition::Quit:
		/*gameMgr->GetRenderWindow().close();*/
		break;
	}
}

PauseMenuState::PauseMenuState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"),
	m_menu(Vector2f({ GameConstants::ScreenDim.x * 0.8f, GameConstants::ScreenDim.y * 0.4f }), 2.f, Vector2f(1,3), MenuPositionData(MenuPositionMode::Centered, GameConstants::ScreenDim / 2.f))
{
	m_gameMgr = gameMgr;
}

void PauseMenuState::Initialise()
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

	if (cell)
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Resume");
		}
		cell->SetMenuSlotNumber(0);
	}

	cell = m_menu.GetCell({ 1,2 });

	if (cell)
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Return To Title Screen");
		}
		cell->SetMenuSlotNumber(1);
	}

	cell = m_menu.GetCell({ 1,3 });

	if (cell)
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Quit");
		}
		cell->SetMenuSlotNumber(2);
	}

	m_menu.SetActiveCells();

	auto menuNav = m_menu.GetMenuNav();
	if (menuNav)
	{
		menuNav->SetCursorRange({ 0,1,2 });
		menuNav->SetCurrCursorPos(0);
	}
}

void PauseMenuState::Pause()
{
}

void PauseMenuState::Resume()
{}

void PauseMenuState::ProcessInputs()
{
	auto inputMgr = GameManager::Get()->GetInputManager();

	if (inputMgr->GetKeyState(static_cast<int>(KeyCode::Enter)))
	{
		switch (m_menu.GetMenuNav()->GetCurrCursorPos())
		{
		case MenuPosition::Resume:
			m_gameMgr->GetGameStateMgr()->PopState();
			break;
		case MenuPosition::ToTitle:
			m_gameMgr->GetGameStateMgr()->ClearStates();
			GameConstants::GameIsReady = false;
			m_gameMgr->GetGameStateMgr()->ChangeState(new TitleState(m_gameMgr));
			break;
		case MenuPosition::Quit:
			m_gameMgr->GetRenderer()->GetWindow()->Close();
			break;
		}
	}
}

void PauseMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void PauseMenuState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();
	if (renderer)
	{
		m_backgroundSpr.Render(renderer);

		m_menu.Render(renderer);
	}
}
