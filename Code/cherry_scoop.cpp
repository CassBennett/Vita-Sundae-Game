#include "cherry_scoop.h" 



// ***************************************************
// Cherry Scoop Constructor
// Sets up the cherry body and sprite, as well as setting the scoop type to cherry
// Takes a Box2d World variable in order to initialise the scoop
// ***************************************************

CherryScoop::CherryScoop(b2World* world_)
{
	// Set up Cherry Scoop body and sprite
	body_dimensions_ = b2Vec2(60.0f, 60.0f);
	scoop_sprite_.set_width(60.0f);
	scoop_sprite_.set_height(60.0f);
	body_initial_position_ = b2Vec2(GFX_BOX2D_POS_X(-100.0f), GFX_BOX2D_POS_Y(-100.0f));
	scoop_sprite_.set_position(Vector3(-100.0f, -100.0f, 0.0f));

	scoop_body_def.type = b2_dynamicBody;
	scoop_body_def.position = body_initial_position_;
	scoop_ = world_->CreateBody(&scoop_body_def);
	scoop_box_.SetAsBox(0.5 * GFX_BOX2D_SIZE(body_dimensions_.x), 0.5 * GFX_BOX2D_SIZE(body_dimensions_.y));
	
	// Fixture
	scoop_fixture_def.shape = &scoop_box_;
	scoop_fixture_def.density = 0.8f;
	scoop_fixture_def.friction = 10.0f;
	scoop_fixture_def.restitution = 0.2f; 
	scoop_->CreateFixture(&scoop_fixture_def);
	scoop_->SetAngularDamping(400.0f);

	// While offscreen and at top of screen without being used set awake to false so it isn't affected by gravity or box2d forces
	scoop_->SetAwake(false);

	scoop_sprite_.SetScoopType(GameObject::CHERRY);
}



// ***************************************************
// Set Texture function
// Called when a cherry scoop is initialised, sets the texture of the next scoop to the cherry sprite
// ***************************************************

void CherryScoop::SetTexture()
{
	scoop_sprite_.TextureSettings(Vector2(0.15, 0.4), 0.075, 0.05f);
}



// ***************************************************
// Update function
// Updates cherry scoop to set awake to false as long as cherry isn't actively being used to keep it in place
// ***************************************************
void CherryScoop::Update()
{
	if (!cherry_active_)
	{
		scoop_->SetAwake(false);
	}
}