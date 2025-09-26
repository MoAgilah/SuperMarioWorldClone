#pragma once

#include <Drawables/SFSprite.h>
#include <Drawables/SFText.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

class TitleState : public IGameState
{
public:
	explicit TitleState(GameManager* gameMgr);
	~TitleState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	SFSprite m_backgroundSpr;
	SFAnimatedText m_titleMessage;
};