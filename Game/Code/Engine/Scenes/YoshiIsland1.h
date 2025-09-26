#pragma once

#include <Engine/Interface/Scene/IScene.h>
#include <Utilities/Vector.h>
#include <memory>
#include <array>

class SFText;
class SFSprite;

enum class Texts { Name, Time, Max };
enum class Sprites { Pipe1, Pipe2, Pipe3, Name, Time, Max};

class YoshiIsland1 : public IScene
{
public:
	YoshiIsland1();
	~YoshiIsland1() = default;

	void Update(float deltaTime) override;
	void Render(IRenderer* renderer) override;
	void ResetScene() override;

	void CheckIsInView();

	void AddObject(const Vector2f& pos);

	void AddEnemies();
	void AddGUI();
	void AddObjects();
	void AddForeGroundSprites();

private:

	void UpdateGUI(float deltaTime);
	void RenderGUI(IRenderer* renderer);
};