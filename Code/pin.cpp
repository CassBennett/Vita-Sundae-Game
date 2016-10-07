#include "pin.h"



// ***************************************************
// Pin Constructor
// Creates the pin bodies and creates a revolute joint, and initialises the colour and rotation
// Takes in a b2World object, and x and y variables to set up the pins
// ***************************************************
Pin::Pin(b2World* world, float x, float y)
{
	world_ = world;
	pin_colour_ = rand() % 3 + 1;
	GetPinColour();
	pin_angle_ = 0.0f;

	pin_position_ = b2Vec2(GFX_BOX2D_POS_X(x), GFX_BOX2D_POS_Y(y));
	pin_dimensions_ = b2Vec2(GFX_BOX2D_SIZE(10.0f), GFX_BOX2D_SIZE(10.0f));
	pin_def_.position = pin_position_;
	pin_body_ = world->CreateBody(&pin_def_);
	pin_shape_.SetAsBox(0.5*pin_dimensions_.x, 0.5 * pin_dimensions_.y);
	pin_body_->CreateFixture(&pin_shape_, 0.0f);

	// Spinning pin circle body
	pin_dimensions_ = b2Vec2(GFX_BOX2D_SIZE(15.0f), GFX_BOX2D_SIZE(15.0f));
	body_def_.type = b2_dynamicBody;
	body_def_.position = pin_position_;
	dynamic_pin_body_ = world->CreateBody(&body_def_);
	dynamic_circle_.m_radius = 0.2f;

	fixture_def_.shape = &dynamic_circle_;
	fixture_def_.density = 0.3f;
	fixture_def_.friction = 0.0f;
	fixture_def_.restitution = 0.0f; 
	dynamic_pin_body_->CreateFixture(&fixture_def_);

	pin_sprite_.set_width(30.0f);
	pin_sprite_.set_height(30.0f);
	pin_sprite_.set_position(Vector3(x, y , 0.0f));
	gfx_pin_position_ = b2Vec2(BOX2D_GFX_POS_X(x), BOX2D_GFX_POS_Y(y));

	// Set values for revolute joint
	revolute_joint_.revolute_joint_struct_.lower_angle = 0.0f;
	revolute_joint_.revolute_joint_struct_.upper_angle = 0.0f;
	revolute_joint_.revolute_joint_struct_.limit_on = false;
	revolute_joint_.revolute_joint_struct_.max_torque = 0.0f;
	revolute_joint_.revolute_joint_struct_.motor_speed = 10.0f;
	revolute_joint_.revolute_joint_struct_.motor_on = true;

	// Creates revolute joint between static body and the circle pin body so it can rotate
	revolute_joint_.CreateRevoluteJoint(world, pin_body_, dynamic_pin_body_);
}



// ***************************************************
// Update function
// Sets the sprites rotation to the same rotation of the dynamic circle body
// ***************************************************

void Pin::Update()
{
	pin_sprite_.RotateSpriteToBody(dynamic_pin_body_, 1000000);
	/*
	pin_angle_ = 1000000 * -(dynamic_pin_body_->GetAngle());
	pin_sprite_.set_rotation(pin_angle_);*/
}



// ***************************************************
// Get Pin Colour
// Determines the pin colour from a random number
// ***************************************************

void Pin::GetPinColour()
{
	switch (pin_colour_)
	{
		case RED_PIN:
			pin_v_ = 0.4f;
			break;
		case BLUE_PIN:
			pin_v_ = 0.45f;
			break;
		case YELLOW_PIN:
			pin_v_ = 0.5f;
			break;
	}
}



// ***************************************************
// Set Texture
// Sets the textures of the pins to the correct texture/colour pin
// ***************************************************

void Pin::SetTexture()
{
	pin_sprite_.TextureSettings(Vector2(0.25, pin_v_), 0.05, 0.05);
}