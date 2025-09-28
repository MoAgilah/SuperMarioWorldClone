#include "Engine/Core/MarioFramework.h"
#include <Engine/Core/Constants.h>

int main()
{
	GameConstants::Init();
	MarioFramework framework;

	return framework.Run();
}