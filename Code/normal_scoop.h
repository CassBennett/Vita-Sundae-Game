#ifndef NORMAL_SCOOP_H
#define NORMAL_SCOOP_H

#include <stdlib.h>
#include <time.h>
#include "scoops.h"

class NormalScoop : public Scoops
{
public:
	// Functions
	NormalScoop(b2World* world_, float x_position, float y_position, int ID);
	void Update();
	void GetNormalScoopColour();
	void SetTexture();

	// Texture Variables
	float normal_scoop_u_;
	int normal_scoop_colour_;

private:

};

#endif