#include "state.h"



// ***************************************************
// State constructor
// Initialises variables for all states in the way they need to be when the game is ran
// ***************************************************

State::State()
{
	state_changed_ = false;
	new_state_ = 0;
	start_screen_on_ = false;
	hard_difficulty_on_ = false;
	sound_FX_on_ = true;
}



// ***************************************************
// Option Selected function
// Detects if the touch position is within the sprites area and returns a true or false depending on this
// Function is used by states when players will click buttons
// ***************************************************

bool State::OptionSelected(GameObject option_box, Touch *touch)
{
	touch_screen_position_x = touch->position.x;
	touch_screen_position_y = touch->position.y;

	box_position_ = Vector3(option_box.position());
	half_width_ = (option_box.width()/2);
	half_height_ = (option_box.height()/2);

	// Detects if the touch screen position is not outside the game object box
	if	(!(((touch_screen_position_x) < (box_position_.x - half_width_))
					||	((touch_screen_position_x) > (box_position_.x + half_width_))
					||	((touch_screen_position_y) < (box_position_.y - half_height_))
					||	((touch_screen_position_y) > (box_position_.y + half_height_))))
	{
		return true;
	}
	else 
	{
		return false;
	}
}