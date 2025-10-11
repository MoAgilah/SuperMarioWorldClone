#pragma once

#include <Engine/Interface/Scene/IScene.h>
#include <Utilities/Vector.h>
#include <memory>
#include <array>

class SFText;
class SFSprite;

class YoshiIsland1 : public IScene
{
public:
	YoshiIsland1();
	~YoshiIsland1() = default;

	void SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos) override;

protected:
	bool AddEnemies() override;
	bool AddGUI() override;
	bool AddObjects() override;
	bool AddForeGroundObjects() override;

private:

	void UpdateGUI(float deltaTime);
};