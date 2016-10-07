#ifndef PIN_H
#define PIN_H

#include <Box2D/Box2D.h>
#include "box2d_helpers.h"
#include <stdlib.h>
#include <time.h>
#include "game_object.h"
#include "maths\vector3.h"
#include "definitions.h"
#include "joints.h"

// For determining the colours of the pins
#define RED_PIN 1
#define BLUE_PIN 2 
#define YELLOW_PIN 3 

class Pin 
{
public:
	// Functions
	Pin(b2World* world, float x, float y);
	void SetTexture();
	void GetPinColour();
	void Update();

	// General Variables/Objects
	b2World* world_;
	int pin_colour_;
	float pin_v_;
	float pin_angle_;

	// Static part of pin
	b2Body *pin_body_;
	b2BodyDef pin_def_;
	b2Vec2 pin_position_;
	b2Vec2 pin_dimensions_;
	b2PolygonShape pin_shape_;
	b2Vec2 gfx_pin_position_;

	// Spinning dynamic part of pin
	b2Body *dynamic_pin_body_;
	GameObject pin_sprite_;
	b2Vec2 spinning_pin_dimensions_;			
	b2CircleShape dynamic_circle_;	
	b2FixtureDef fixture_def_;	
	b2BodyDef body_def_;

	// Revolute Joint
	Joints revolute_joint_;

private:

};

#endif