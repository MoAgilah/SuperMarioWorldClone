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

	auto cell = m_menu.GetCell({ 1,1 });

	ENSURE_VALID(cell);
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			text->SetText("Resume");
			GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
			sfText->SetIsLooping(true);
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
			text->SetText("Return To Title Screen");
			GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
			sfText->SetIsLooping(true);
		}
		cell->SetMenuSlotNumber(1);
	}

	cell = m_menu.GetCell({ 1,3 });

	ENSURE_VALID(cell);
	{

		TextConfig config;
		config.m_position = cell->GetPosition();

		auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(config));
		if (text)
		{
			text->SetFillColour(Colour::Yellow);
			text->SetText("Quit");
			GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
			sfText->SetIsLooping(true);
		}
		cell->SetMenuSlotNumber(2);
	}

	m_menu.SetActiveCells();

	GET_OR_RETURN(menuNav, m_menu.GetMenuNav());

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
	GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

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

			GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());
			GET_OR_RETURN(window, renderer->GetWindow());
			window->Close();
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
	GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_menu.Render(renderer);
}
