#include "game_object.h"




// ***************************************************
// Game Object constructor
// Initialises variables to normal game object
// ***************************************************

GameObject::GameObject()
{
	type_ = INIT;
	uv_position_.x = 0;
	uv_position_.y = 0;
	animation_counter_ = 1;
	ID_ = 0;
}



// ***************************************************
// Give ID function
// The ID is used for determining which scoop is being used from the gameobject information when detecting collision
// Takes in ID Number which is assigned to the appropriate game objects at creation
// ***************************************************

void GameObject::GiveID(int ID_number)		// This is for game objects in arrays to use with collision detection
{
	ID_ = ID_number;
}



// ***************************************************
// Set Object Type function
// Sets the object type of the game object in order to differentiate between different objects when detecting collision
// Takes in a ObjectType variable in order to set the type to the appropriate type
// ***************************************************

void GameObject::SetObjectType(ObjectType type)
{
	type_ = type;
}



// ***************************************************
// Set Scoop Type function
// Sets the scoop type of the game object of a scoop to differentiate between different scoops when detecting collision
// Takes in a ScoopType variable in order to set the type to the appropriate type
// ***************************************************

void GameObject::SetScoopType(ScoopType type)
{
	scoop_type_ = type;
}



// ***************************************************
// Give Texture function
// Function takes in a Texture object and these are passed in when a game object is created, so the texture can be accessed by multiple classes
// Takes in a Texture object
// ***************************************************

void GameObject::GiveTexture(Texture *sprite_sheet)
{
	sprite_texture_ = sprite_sheet;
}



// ***************************************************
// Get Position function
// Returns position of game object in terms of Vita screen position
// ***************************************************

Vector3 GameObject::GetPosition()
{
	return position_;
}



// ***************************************************
// Texture Settings function
// Assigns the given texture to the sprite and sets up the uv position, width and height to display the texture correctly
// Takes in the sprite to be given the texture, the uv position, and the desired uv width and height
// ***************************************************

void GameObject::TextureSettings(Vector2 uv_position, float uv_width, float uv_height)
{
	set_texture(sprite_texture_);
	set_uv_position(Vector2(uv_position.x, uv_position.y));
	set_uv_width(uv_width);
	set_uv_height(uv_height);
	uv_position_.x = uv_position.x;
	uv_position_.y = uv_position.y;
	unchanged_uv_position_.y = uv_position.y;
}



// ***************************************************
// Change Texture function
// Uses the texture already assigned to the sprite and allows for program to change the u and v position 
// and sets unchanged v position and height for use with animation
// Takes in values for the u position and v position
// ***************************************************

void GameObject::ChangeTexture(float uv_x, float uv_y)
{
	set_uv_position(Vector2(uv_x, uv_y));
	uv_position_.x = uv_x;
	uv_position_.y = uv_y;
	unchanged_uv_position_.y = uv_y;
	uv_height_ = uv_height();
}



// ***************************************************
// Rotate Sprite to Body function
// Takes in a pointer to a box2d body object, and sets the sprite rotation angle to the angle of the body
// Used to synchronise sprite rotation with body rotation
// ***************************************************

void GameObject::RotateSpriteToBody(b2Body *body, float rate)
{
	float body_angle = rate * -(body->GetAngle());
	set_rotation(body_angle);
}



// ***************************************************
// Get X function
// Returns X position of sprite on screen
// ***************************************************

float GameObject::GetX()
{
	return position_.x;
}



// ***************************************************
// Get Y function
// Returns Y position of sprite on screen
// ***************************************************

float GameObject::GetY()
{
	return position_.y;
}



// ***************************************************
// Resize Sprite function
// Takes in values for new width and new height values and changes the sprite
// ***************************************************
void GameObject::ResizeSprite(float new_width, float new_height)
{
	set_width(new_width);
	set_height(new_height);
}




// ***************************************************
// Animate function
// Uses the uv position of the texture, the number of frames set by the game object, and animation counter to animate the sprite
// Moves through the v direction of the spritesheet to animate a sprite up to the given limit
// ***************************************************
void GameObject::Animate()
{
	// Sets the limit for each sprite animation to begin at the normal uv y position for sprites whose animation isn't at the top of the spritesheet
	float limit = (unchanged_uv_position_.y + (num_frames_ - 1) * uv_height());
	animation_counter_++;

	// When the animation counter is dividable by the dependant frame count
	if ((animation_counter_) % game_frame_count_ == 0)
	{
		uv_position_.y += uv_height();

		// Sets the v position of the texture to the original if the animation reaches the limit so it begins again
		if	(uv_position_.y > limit)		
		{
			uv_position_.y = unchanged_uv_position_.y;
			set_uv_position(Vector2(uv_position_.x, uv_position_.y));
		}
		else
		{
			set_uv_position(Vector2(uv_position_.x, uv_position_.y));
		}
	}
}