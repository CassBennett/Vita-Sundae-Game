#ifndef SPOON_MANAGER_H
#define SPOON_MANAGER_H

#include "spoon.h"
#include "definitions.h"
#include "scoop_manager.h"
#include "game_object.h"
#include <graphics/texture.h>

class SpoonManager
{
public:
	// Functions
	SpoonManager(b2World* world_, Texture *spritesheet);
	void Update(ScoopManager *scoop_manager);
	void InitialiseConveyorBelts();

	// Spoon Objects / Variables
	Spoon *spoons[MAX_SPOONS];
	int scoops_placed_;
	int spoons_finished_;

	// Reference Static Body (for mouse joints)
	b2Body *reference_body_;
	b2BodyDef reference_def_;
	b2Vec2 reference_position_;
	b2Vec2 reference_dimensions_;
	b2PolygonShape reference_shape_;

	// Spoon conveyor belt
	bool spoons_moving_;
	Texture *spritesheet_;
	GameObject conveyor_belt_1_;
	GameObject conveyor_belt_2_;
private:

};

#endif
