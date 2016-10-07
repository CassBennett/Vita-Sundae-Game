#ifndef SCOOP_MANAGER_H
#define SCOOP_MANAGER_H

#include "scoops.h"
#include "normal_scoop.h"
#include "bomb_scoop.h"
#include "cherry_scoop.h"
#include "maths\vector2.h"

class ScoopManager
{
public:
	// Functions
	ScoopManager(b2World* world_);
	void Update(b2World* world_);
	void SetScoopType();
	void NewScoops(b2World* world_);

	// Collision Response Functions
	void ScoopPlaced(int scoop_ID);
	void ScoopPlacedOnGround(int scoop_ID);
	void ScoopCollidesWithBomb(Scoops *bomb_scoop, Scoops *normal_scoop);

	// Scoop Objects
	Scoops *scoop[NUM_OF_SCOOPS];
	Scoops *cherry_;

	// General Variables/Objects
	int spoon_timer_;
	int current_scoop_amount_;
	bool call_new_scoops_;
	int scoops_fallen_;
	bool cherry_active_;
	Vector2 explosion_direction_;
private:

};

#endif