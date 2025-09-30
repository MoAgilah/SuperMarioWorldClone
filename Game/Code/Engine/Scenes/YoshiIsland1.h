#pragma once

#include <Engine/Interface/Scene/IScene.h>
#include <Utilities/Vector.h>
#include <memory>
#include <array>

class SFText;
class SFSprite;

enum class Texts { Name, Time, Max };
enum class Sprites { Name, Time, Max};

class YoshiIsland1 : public IScene
{
public:
	YoshiIsland1();
	~YoshiIsland1() = default;

	void SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos) override;

	void AddEnemies();
	void AddGUI();
	void AddObjects();
	void AddForeGroundObjects();

private:

	void UpdateGUI(float deltaTime);
};