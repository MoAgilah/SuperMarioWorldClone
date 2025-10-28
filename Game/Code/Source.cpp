#include "Engine/Core/MarioFramework.h"
#include <Engine/Core/Constants.h>

int main()
{
	GameConstants::Init();

#if defined(_DEBUG) || defined(DEBUG)
	GameConstants::DRender = true;
#else
	GameConstants::DRender = false;
#endif

	MarioFramework framework;

	return framework.Run();
}