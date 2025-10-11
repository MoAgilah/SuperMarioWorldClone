#include "PauseMenuState.h"

#include "../../Utilities/GameMode.h"
#include <Drawables/SFText.h>
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>

enum MenuPosition { Resume, ToTitle, Quit };

PauseMenuState::PauseMenuState(GameManager* gameMgr)
	: IGameState(gameMgr), m_backgroundSpr("Title"),
	m_menu(Vector2f({ GameConstants::ScreenDim.x * 0.8f, GameConstants::ScreenDim.y * 0.4f }), 2.f, Vector2f(1,3), MenuPositionData(MenuPositionMode::Centered, GameConstants::ScreenDim / 2.f))
{}

void PauseMenuState::Initialise()
{
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	auto cellSize = m_menu.GetCellSize();

	TextConfig config;
	config.m_fontName = "SMW";
	config.m_charSize = static_cast<int>(cellSize.y * 0.6f);
	config.m_animType = TextAnimType::Flashing;
	config.m_alignment = TextAlignment::Center;
	config.m_colour = Colour::Black;

	DECL_GET_OR_RETURN(cell, m_menu.GetCell({ 1,1 }));

	config.m_position = cell->GetPosition();

	DECL_GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	text->SetFillColour(Colour::Yellow);
	DECL_GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
	sfText->InitFlashingText("Resume");

	cell->SetMenuSlotNumber(0);

	GET_OR_RETURN(cell, m_menu.GetCell({ 1,2 }));

	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	text->SetFillColour(Colour::Yellow);
	GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
	sfText->InitFlashingText("Return To Title Screen");

	cell->SetMenuSlotNumber(1);

	GET_OR_RETURN(cell, m_menu.GetCell({ 1,3 }));

	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	text->SetFillColour(Colour::Yellow);
	GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
	sfText->InitFlashingText("Quit");

	cell->SetMenuSlotNumber(2);

	m_menu.SetActiveCells();

	DECL_GET_OR_RETURN(menuNav, m_menu.GetMenuNav());

	menuNav->SetCursorRange({ 0,1,2 });
	menuNav->SetCurrCursorPos(0);
}

void PauseMenuState::Pause()
{
}

void PauseMenuState::Resume()
{}

void PauseMenuState::ProcessInputs()
{
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->GetKeyState(static_cast<int>(KeyCode::Enter)))
	{
		switch (m_menu.GetMenuNav()->GetCurrCursorPos())
		{
		case MenuPosition::Resume:
			m_gameMgr->GetGameStateMgr()->PopState();
			break;
		case MenuPosition::ToTitle:
			GameMode::ToTitle();
			break;
		case MenuPosition::Quit:
		{

			DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());
			DECL_GET_OR_RETURN(window, renderer->GetWindow());
			window->Close();
		}
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
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_menu.Render(renderer);
}
