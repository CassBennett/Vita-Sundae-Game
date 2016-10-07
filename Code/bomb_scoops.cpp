#include "bomb_scoop.h"



// ***************************************************
// Bomb Scoop Constructor
// Sets up the bomb body and sprite, as well as setting the scoop type to bomb_scoop and initialising explosion variables
// Takes a Box2d World variable, x position, y position and ID in order to initialise the scoop
// ***************************************************

BombScoop::BombScoop(b2World* world, float x_position, float y_position, int ID)
{
	scoop_ID_ = ID;
	explosion_timer_ = 0;
	explosion_countdown_ = false;
	explosion_now_ = false;

	// Set up bomb scoop body

	body_dimensions_ = b2Vec2(55.0f, 55.0f); 
	scoop_sprite_.set_width(body_dimensions_.x);
	scoop_sprite_.set_height(body_dimensions_.y);
	body_initial_position_ = b2Vec2(GFX_BOX2D_POS_X(x_position), GFX_BOX2D_POS_Y(y_position));
	scoop_sprite_.set_position(Vector3(x_position, y_position, 0.0f));

	scoop_body_def.type = b2_dynamicBody;
	scoop_body_def.position = body_initial_position_;
	scoop_ = world->CreateBody(&scoop_body_def);
	scoop_box_.SetAsBox(0.5*GFX_BOX2D_SIZE(body_dimensions_.x - 10), 0.5*GFX_BOX2D_SIZE(body_dimensions_.y - 10));

	// Fixture
	scoop_fixture_def.shape = &scoop_box_;
	scoop_fixture_def.density = 0.8f;
	scoop_fixture_def.friction = 10.0f;
	scoop_fixture_def.restitution = 0.2f; // bouncy
	scoop_->CreateFixture(&scoop_fixture_def);
	scoop_->SetAngularDamping(400.0f);
	
	scoop_sprite_.SetScoopType(GameObject::BOMB_SCOOP);
	bomb_scoop_ = true;
}



// ***************************************************
// Set Texture function
// Called when a bomb scoop is initialised, sets the texture of the next scoop to the bomb sprite
// ***************************************************

void BombScoop::SetTexture()
{
	scoop_sprite_.TextureSettings(Vector2(0.1, 0.4f), 0.05, 0.05);
}



// ***************************************************
// Update function
// Updates bomb scoop to handle the explosions using timers
// ***************************************************

void BombScoop::Update()
{
	if (explosion_timer_ >= 0)
	{
		explosion_timer_--;
	}

	if (((already_placed_) && (!explosion_countdown_)) && (!exploded_))
	{
		explosion_timer_ = 150;
		ResetState();
		exploding_ = true;
		explosion_countdown_ = true;
	}

	// Stops explosion time and triggers explosion so any bombs in contact within explosion timer 5 to 0 will be affected
	if ((explosion_countdown_) && (explosion_timer_ == 5))
	{ 
		explosion_now_ = true;
		exploding_ = false;
	}

	if ((explosion_countdown_) && (explosion_timer_ <= 0))
	{
		explosion_now_ = false;
		ResetState();
		exploded_ = true;
		explosion_countdown_ = false;
	}
}