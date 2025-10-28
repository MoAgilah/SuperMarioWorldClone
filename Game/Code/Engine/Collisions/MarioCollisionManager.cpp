#include "MarioCollisionManager.h"

#include "../../GameObjects/Bill.h"
#include "../../GameObjects/Chuck.h"
#include "../../GameObjects/Koopa.h"
#include "../../GameObjects/Player.h"
#include "../../GameObjects/Rex.h"
#include "../../GameObjects/Collectables.h"
#include <Utilities/Utils.h>

MarioCollisionManager::MarioCollisionManager(std::shared_ptr<IGrid> grid)
    : ICollisionManager(std::move(grid))
{
    ICollisionManager::s_canCollideWithTile = { typeid(Bill), typeid(Chuck), typeid(Koopa), typeid(Rex), typeid(Player), typeid(Goal), typeid(Mushroom) };
}

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