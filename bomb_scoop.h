#ifndef BOMB_SCOOP_H
#define BOMB_SCOOP_H

#include "scoops.h"

class BombScoop : public Scoops
{
public:
	// Functions
	BombScoop(b2World* world_, float x_position, float y_position, int ID);
	void Update();

private:
	// Functions
	void SetTexture();

	// Variables
	int explosion_timer_;
	bool explosion_countdown_;
};

#endif