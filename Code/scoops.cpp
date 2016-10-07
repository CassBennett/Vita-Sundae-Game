#include "scoops.h"



// ***************************************************
// Scoops constructor
// Initialises everything needed for each scoop such as initial states
// ***************************************************

Scoops::Scoops()
{
	// Inititialisation for scoops here
	activate_area_x_ = SCOOP_X_LIMIT;
	scoop_u_value_ = 0.0f;
	scoop_sprite_.SetObjectType(GameObject::SCOOP);

	// Bool initialisation
	normal_scoop_ = false;
	bomb_scoop_ = false;
	already_placed_ = false;
	can_move_ = false;
	sitting_ = true;
	falling_ = false;
	scared_ = false;
	grabbed_ = false;
	sad_ = false;
	initialised_ = false;
	can_collide_with_plate_ = true;
	can_collide_with_floor_ = true;
	state_can_change_ = true;
	cherry_active_ = false;

	// Bools for bomb
	explosion_now_ = false;
	exploded_ = false;
	exploding_ = false;
	
	// For Animation
	scoop_sprite_.game_frame_count_ = 40;
	scoop_sprite_.num_frames_ = 2;
}

void Scoops::Update()
{

} 



// ***************************************************
// Update All Scoops function
// Updates textures for the scoops, the position/rotation of sprites, and whether the scoop can move or not
// ***************************************************

void Scoops::UpdateAllScoops()
{
	// Update scoop sprite rotation
	scoop_sprite_.RotateSpriteToBody(scoop_, 1.0f);
	//scoop_body_angle = -(scoop_->GetAngle());
	//scoop_sprite_.set_rotation(scoop_body_angle);

	// Update scoop sprite position
	scoop_position_ = &scoop_->GetPosition();
	gfx_scoop_position_ = b2Vec2(BOX2D_GFX_POS_X(scoop_position_->x), BOX2D_GFX_POS_Y(scoop_position_->y));
	scoop_sprite_.set_position(Vector3(gfx_scoop_position_.x, gfx_scoop_position_.y, 0.0f));

	UpdateScoopTexture();

	// Determines if scoop can be moved or not
	if ((already_placed_ == true) || (BOX2D_GFX_POS_X(scoop_position_->x) < SCOOP_X_LIMIT - 30))
	{
		can_move_ = false;
	}
	if ((BOX2D_GFX_POS_X(scoop_position_->x) > activate_area_x_))
	{
		can_move_ = true;
	}
}




// ***************************************************
// Update Scoop Texture function
// Changes the texture of the scoops depending on what state they're in and which type of scoop they are
// ***************************************************

void Scoops::UpdateScoopTexture()
{
	// So it changes the texture of the sprite when a state is changed
	if (state_can_change_)
	{
		if (scoop_sprite_.scoop_type_ == GameObject::NORMAL_SCOOP)
		{	
			if (falling_)
			{
				scoop_sprite_.ResizeSprite(130.0f, 55.0f);
				scoop_sprite_.set_uv_width(0.075f);
				scoop_sprite_.set_uv_position(Vector2(scoop_u_value_, 0.2f));
				scoop_sprite_.ChangeTexture(scoop_u_value_, 0.2f);
			}
			else 
			{
				scoop_sprite_.ResizeSprite(55.0f, 55.0f);
				scoop_sprite_.set_uv_width(0.05);

				if (sitting_)
				{
					scoop_sprite_.ChangeTexture(scoop_u_value_, 0.0f);
				}
				if (scared_)
				{
					scoop_sprite_.ChangeTexture(scoop_u_value_, 0.1f);
				}
				if (grabbed_)
				{
					scoop_sprite_.ChangeTexture(scoop_u_value_, 0.15f);
				}
				if (sad_)
				{
					scoop_sprite_.ChangeTexture(scoop_u_value_, 0.3f);
				}
			}
		}
	
		if (scoop_sprite_.scoop_type_ == GameObject::BOMB_SCOOP)
		{
			if (falling_)
			{
				scoop_sprite_.ResizeSprite(130.0f, 55.0f);
				scoop_sprite_.set_uv_width(0.075f);
				scoop_sprite_.set_uv_position(Vector2(0.1f, 0.5f));
				scoop_sprite_.ChangeTexture(0.1f, 0.5f);	
			}
			if (exploding_)
			{
				scoop_sprite_.ResizeSprite(90.0f, 55.0f);
				scoop_sprite_.set_uv_width(0.075f);
				scoop_sprite_.set_uv_position(Vector2(0.1f, 0.6f));
				scoop_sprite_.ChangeTexture(0.1f, 0.6f);
			}
			if (exploded_)
			{
				scoop_sprite_.ResizeSprite(55.0f, 55.0f);
				scoop_sprite_.set_uv_width(0.05);
				scoop_sprite_.set_uv_position(Vector2(0.1, 0.7f));
				scoop_sprite_.ChangeTexture(0.1f, 0.7f);
			}
		}
	
		state_can_change_ = false;
	}

	if (((sitting_) || (sad_)) || (exploded_))
	{
		scoop_sprite_.game_frame_count_ = 40;
	}

	if ((falling_) || (exploding_))
	{
		scoop_sprite_.game_frame_count_ = 20;
	}

	// Animate scoops 
	if ((!scared_) && (!grabbed_))
	{
		scoop_sprite_.Animate();
	}
}



// ***************************************************
// Reset State function
// Changes all states to false so it can be changed
// ***************************************************

void Scoops::ResetState()
{
	sitting_ = false;
	falling_ = false;
	scared_ = false;
	sad_ = false;
	grabbed_ = false;
	state_can_change_ = true;
}



void Scoops::SetTexture()
{

}



// ***************************************************
// Affected By Bomb function
// Applies a linear impulse to the scoops colliding with a bomb at explosion time so they are pushed away 
// Takes in the bomb scoops position in order to find the direction the other scoops should be pushed away in
// ***************************************************

void Scoops::AffectedByBomb(b2Vec2 bomb_position)
{
	scoop_sprite_.set_colour(0xff000000);	
	// Get the direction vector for the direction the explosion should push the scoop in
	explosion_direction_.x = (gfx_scoop_position_.x - bomb_position.x);
	explosion_direction_.y = -(gfx_scoop_position_.y - bomb_position.y);
	explosion_direction_.Normalize();
	explosion_direction_ *= 1.0f;

	// Apply the impulse to the scoop
	scoop_->ApplyLinearImpulse(b2Vec2(explosion_direction_.x, explosion_direction_.y), scoop_->GetWorldCenter());
}