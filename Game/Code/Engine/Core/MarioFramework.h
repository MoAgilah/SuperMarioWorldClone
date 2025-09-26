#pragma once

#include <Core/SFFramework.h>

class MarioFramework : public SFFramework
{
public:
	MarioFramework();

    void Initialise() override;
    void Shutdown() override;
};