#ifndef SCOOPS_H
#define SCOOPS_H

#include <Box2D/Box2D.h>
#include "box2d_helpers.h"
#include "game_object.h"
#include "maths\vector3.h"
#include "definitions.h"
#include "box2d_helpers.h"

// Definitions for the different coloured normal scoops
#define BLUE_SCOOP 1
#define PINK_SCOOP 2 
#define YELLOW_SCOOP 3 
#define GREEN_SCOOP 4
#define ORANGE_SCOOP 5

class Scoops 
{
public:
	// Functions
	Scoops();
	void UpdateAllScoops();
	void UpdateScoopTexture();	
	void ResetState();
	void AffectedByBomb(b2Vec2 bomb_position);
	// Virtual voids for the Scoops
	virtual void SetTexture();
	virtual void Update();

	// Box2d & Sprite Variables/Objects
	GameObject scoop_sprite_;
	b2Vec2 body_initial_position_;
	b2Vec2 body_dimensions_;
	b2Body *scoop_;	
	b2PolygonShape scoop_box_;			
	b2FixtureDef scoop_fixture_def;
	b2BodyDef scoop_body_def;
	const b2Vec2 *scoop_position_;
	b2Vec2 gfx_scoop_position_;
	float scoop_body_angle;

	// General Variables/Objects
	int scoop_ID_;
	float activate_area_x_; 
	float scoop_u_value_;
	
	// State bools
	bool can_move_;
	bool sitting_;
	bool falling_;
	bool scared_;
	bool grabbed_;
	bool sad_;
	bool state_can_change_;
	bool already_placed_;
	bool can_collide_with_plate_;
	bool can_collide_with_floor_;
	bool initialised_; 

	// Bools for each type (for collision detection)
	bool normal_scoop_;
	bool bomb_scoop_;
	bool cherry_active_;

	// For bombs (Needs to be here for use in collision detection)
	bool explosion_now_;
	bool exploded_;
	bool exploding_;
	b2Vec2 explosion_direction_;

private:

};

#endif