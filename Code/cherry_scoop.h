#ifndef CHERRY_SCOOP_H
#define CHERRY_SCOOP_H

#include "scoops.h"
#include "box2d_helpers.h"

class CherryScoop : public Scoops
{
public:
	// Functions
	CherryScoop(b2World* world_);
	void Update();

private:
	// Functions
	void SetTexture();
};

#endif