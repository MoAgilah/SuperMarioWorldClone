#pragma once

#include <Drawables/SFSprite.h>
#include <Drawables/SFText.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

void LoadResources();

class LoadingState : public IGameState
{
public:
	explicit LoadingState(GameManager* gameMgr);
	~LoadingState() override = default;

	std::string_view GetStateName() const { return "Loading"; }

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