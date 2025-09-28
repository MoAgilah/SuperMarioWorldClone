#pragma once

#include <Engine/Interface/Collisions/ICollisionManager.h>

class MarioCollisionManager : public ICollisionManager
{
public:
	MarioCollisionManager(std::shared_ptr<IGrid> grid);
	~MarioCollisionManager() = default;

private:

	void DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time) override;
	void DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time) override;
};