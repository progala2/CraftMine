#ifndef _XKS_CREATURE_
#define _XKS_CREATURE_
#include "stdafx.h"

class Creature
{
public:
	Creature() {}

	virtual void Draw() = 0;
	virtual void Update(double dt) = 0;
};

#endif