#ifndef SPOON_H
#define SPOON_H

#include <Box2D/Box2D.h>
#include "box2d_helpers.h"
#include "game_object.h"
#include "maths\vector3.h"
#include "joints.h"
#include <iostream>

class Spoon
{
public:
	// Functions
	Spoon(b2World* world_, bool left_spoon, bool right_spoon, float x_position, float y_position);
	void Update();
	void MoveSpoon();
	void SetTexture();
	void SetUpMouseJoint(b2Body *ref_body, b2World* world);

	// Box2d & Sprite Variables
	b2Body *spoon_;	
	b2PolygonShape spoon_box_;			
	b2FixtureDef spoon_fixture_def;
	b2BodyDef spoon_body_def;
	b2Vec2 body_initial_position_;
	b2Vec2 body_dimensions_;	
	GameObject spoon_sprite_;
	b2Vec2 spoon_position_;
	b2Vec2 graphics_spoon_position_;

	// Spoon direction variables
	bool left_spoon_;
	bool right_spoon_;
	bool collide_with_ceiling_;

	// Initial values
	float original_y_position_;
	float off_screen_y_;

	// Positions the spoon will move to
	b2Vec2 starting_position_;
	b2Vec2 second_position_;
	b2Vec2 off_screen_position_;
	b2Vec2 at_top_position_;
	int scoops_placed_;
	
	// Mouse Joint
	Joints mouse_joint_;

	// Variables for moving
	bool can_move_;
	bool move_to_first_;
	bool move_to_second_;
	bool move_to_start_;
	bool move_to_top_;
	bool at_start_;
	bool at_first_;
	bool at_second_;
	int move_delay_;
};


#endif