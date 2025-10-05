#include "MarioCollisionManager.h"

#include <Utilities/Utils.h>

MarioCollisionManager::MarioCollisionManager(std::shared_ptr<IGrid> grid)
    : ICollisionManager(std::move(grid))
{}

void MarioCollisionManager::DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time)
{
    ENSURE_VALID(obj1);
    ENSURE_VALID(obj2);
}

void MarioCollisionManager::DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time)
{
    ENSURE_VALID(obj1);
    ENSURE_VALID(obj2);
}