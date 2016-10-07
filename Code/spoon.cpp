#include "spoon.h"



// ***************************************************
// Spoon Constructor
// Initialises a spoon depending on the type of spoon passed in by the Spoon Manager 
// Takes a pointer to a box2d world object, bools to determine if it s a left or right spoon, and values of the x and y position
// ***************************************************

Spoon::Spoon(b2World* world, bool left_spoon, bool right_spoon, float x_position, float y_position)
{
	left_spoon_ = left_spoon;
	right_spoon_ = right_spoon;

	// Set normal y positions and off screen y positions 
	if (left_spoon_)
	{
		original_y_position_ = 63.0f;
		off_screen_y_ = -70;
	}

	if (right_spoon_)
	{
		original_y_position_ = 477.0f;
		off_screen_y_ = 610.0f;
	}

	// Set up body/sprite of the spoon
	body_dimensions_ = b2Vec2(50.0f, 110.0f);  
	spoon_sprite_.set_width(body_dimensions_.x);
	spoon_sprite_.set_height(body_dimensions_.y);
	body_initial_position_ = b2Vec2(GFX_BOX2D_POS_X(x_position), GFX_BOX2D_POS_Y(y_position));
	spoon_sprite_.set_position(Vector3(original_y_position_, BOX2D_GFX_POS_Y(body_initial_position_.y), 0.0f));

	spoon_body_def.type = b2_dynamicBody;
	spoon_body_def.position = body_initial_position_;
	spoon_body_def.fixedRotation = true;
	spoon_ = world->CreateBody(&spoon_body_def);
	spoon_box_.SetAsBox(0.5*GFX_BOX2D_SIZE(body_dimensions_.x), 0.5*GFX_BOX2D_SIZE(body_dimensions_.y  - 10));

	spoon_fixture_def.shape = &spoon_box_;
	spoon_fixture_def.density = 25.0f;
	spoon_fixture_def.friction = 3.0f;
	spoon_fixture_def.restitution = 0.1f;
	spoon_->CreateFixture(&spoon_fixture_def);

	spoon_->SetUserData(&spoon_sprite_);

	// Initialisation
	move_delay_ = 0;
	can_move_ = false;
	move_to_first_ = false;
	move_to_second_ = false;
	move_to_start_ = false;
	move_to_top_ = false;
	at_start_ = false;
	at_first_ = false;
	at_second_ = false;
	collide_with_ceiling_ = true;

	// Set positions the spoon will move to based on the original y and off screen y assigned to it
	starting_position_ = b2Vec2(GFX_BOX2D_POS_X(650.0f), GFX_BOX2D_POS_Y(original_y_position_));
	second_position_ = b2Vec2(GFX_BOX2D_POS_X(800.0f), GFX_BOX2D_POS_Y(original_y_position_));
	off_screen_position_ = b2Vec2(GFX_BOX2D_POS_X(650.0f), GFX_BOX2D_POS_Y(off_screen_y_));
	at_top_position_ = b2Vec2(GFX_BOX2D_POS_X(DISPLAY_WIDTH), GFX_BOX2D_POS_Y(original_y_position_));

	// Set value for mouse joint 
	mouse_joint_.mouse_joint_struct_.max_force_ = 17.0f;
}



// ***************************************************
// Set Texture
// Sets the texture of the spoon based on the type of spoon it is 
// ***************************************************

void Spoon::SetTexture()
{
	if (left_spoon_)
	{
		spoon_sprite_.TextureSettings(Vector2(0.35, 0.65f), 0.05, 0.1);
	}

	if (right_spoon_)
	{
		spoon_sprite_.TextureSettings(Vector2(0.3, 0.65f), 0.05, 0.1);
	}
}


// ***************************************************
// Set Up Mouse Joint function
// Creates the mouse joint for the spoon
// Takes in a pointer to the reference body position and a pointer to a box2d world object to pass into the Create Mouse Joint function
// ***************************************************

void Spoon::SetUpMouseJoint(b2Body *ref_body, b2World* world)
{
	mouse_joint_.CreateMouseJoint(world, ref_body, spoon_);
}



// ***************************************************
// Update function
// Updates the sprite position to be the spoons body position and calls MoveSpoon
// ***************************************************

void Spoon::Update()
{
	spoon_position_ = spoon_->GetPosition();
	spoon_sprite_.set_position(Vector3(BOX2D_GFX_POS_X(spoon_position_.x), BOX2D_GFX_POS_Y(spoon_position_.y), 0.0f));
	MoveSpoon();
}



// ***************************************************
// Move Spoon function
// Detects which stage the spoon is at and sets bools to carry out movement to different positions on the screen
// ***************************************************

void Spoon::MoveSpoon()
{
	graphics_spoon_position_ = b2Vec2(BOX2D_GFX_POS_X(spoon_position_.x), BOX2D_GFX_POS_Y(spoon_position_.y));

	if (move_delay_ >= 0)
	{
		move_delay_--;
	}

	// Moves to the first position
	if (move_to_first_)
	{
		move_to_start_ = false;
		mouse_joint_.mouse_joint_->SetTarget(starting_position_);
		if (graphics_spoon_position_.x >= BOX2D_GFX_POS_X(starting_position_.x) - 20)
		{
			at_first_ = true;
		}
	}

	// Moves to second position
	if (move_to_second_)
	{
		move_to_first_ = false;
		at_first_ = false;
		mouse_joint_.mouse_joint_->SetTarget(second_position_);
		if (graphics_spoon_position_.x >= BOX2D_GFX_POS_X(second_position_.x) - 20)
		{
			at_second_ = true;
		}
	}

	if (move_to_start_)
	{
		move_to_top_ = false;
		at_second_ = false;
		// Uses SetTransform to quickly move spoon to the original position offscreen
		spoon_->SetTransform(off_screen_position_, 0.0f);
		collide_with_ceiling_ = true;
		if (move_delay_ <= 0)
		{
			if (graphics_spoon_position_.x >= BOX2D_GFX_POS_X(off_screen_position_.x) - 20)
			{
				at_start_ = true;
			}
			move_to_first_ = true;
		}
	}
	
	if ((at_second_) && (move_to_top_))
	{
		move_to_second_ = false;
		mouse_joint_.mouse_joint_->SetTarget(at_top_position_);
	}
}