#include "normal_scoop.h"



// ***************************************************
// Normal Scoop Constructor
// Sets up the normal scoop body and sprite, as well as setting the scoop type to normal_scoop and initialising variables
// Takes a Box2d World variable, x position, y position and ID in order to initialise the scoop
// ***************************************************

NormalScoop::NormalScoop(b2World* world, float x_position, float y_position, int ID)
{
	// Sets random scoop colour
	normal_scoop_colour_ = rand() % 5 + 1;
	scoop_ID_ = ID;
	normal_scoop_u_ = 0.0f;

	// Initialises body
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

	scoop_sprite_.SetScoopType(GameObject::NORMAL_SCOOP);
	normal_scoop_ = true;
	GetNormalScoopColour();
}



// ***************************************************
// Get Normal Scoop Colour function
// Set the u value for textures based on the random number for the scoop colour
// ***************************************************

void NormalScoop::GetNormalScoopColour()
{
	switch (normal_scoop_colour_)
	{
		case BLUE_SCOOP:
			normal_scoop_u_ = 0.0f;
			break;
		case PINK_SCOOP:
			normal_scoop_u_ = 0.1f;
			break;
		case YELLOW_SCOOP:
			normal_scoop_u_ = 0.2f;
			break;
		case GREEN_SCOOP:
			normal_scoop_u_ = 0.3f;
			break;
		case ORANGE_SCOOP:
			normal_scoop_u_ = 0.4f;
			break;
	}
}



// ***************************************************
// Set Texture function
// Called when a scoop is initialised, sets the texture of the next scoop to the appropriate sprite
// ***************************************************

void NormalScoop::SetTexture()
{
	scoop_sprite_.TextureSettings(Vector2(normal_scoop_u_, 0.0f), 0.05, 0.05);
	scoop_u_value_ = normal_scoop_u_;
}



// Here because it's a virtual function within Scoops
void NormalScoop::Update()
{


}