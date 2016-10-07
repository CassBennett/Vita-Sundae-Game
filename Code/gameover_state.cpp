#include "gameover_state.h"



// ***************************************************
// GameOverState Constructor
// Initialises variables needed when the game is first ran
// ***************************************************

GameOverState::GameOverState()
{
	in_state_ = false;
	state = GAMEOVER_STATE;
	state_changed_ = false;
}



// ***************************************************
// On Enter function
// Different to state constructor because these are things to be initialised every time this state is entered
// ***************************************************

void GameOverState::OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform)
{
	in_state_ = true;
	state = GAMEOVER_STATE;
	state_changed_ = false;
	new_state_ = GAMEOVER_STATE;

	// Set up GameObjects
	background_.set_width(DISPLAY_WIDTH);
	background_.set_height(DISPLAY_HEIGHT);
	background_.GiveTexture(spritesheet);
	background_.TextureSettings(Vector2(0.5, 0.6), 0.25, 0.15);
	background_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	menu_sprite_.set_width(70.0f);
	menu_sprite_.set_height(150.0f);
	menu_sprite_.GiveTexture(spritesheet);
	menu_sprite_.TextureSettings(Vector2(0.9, 0.05), 0.05, 0.1);
	menu_sprite_.set_position(Vector3(90.0f, 100.0f, 0.0f));
}



// ***************************************************
// Update function
// Gets the touch screen position and detects if player has selected the menu button
// Takes in float for frame rate, Touch object and AudioManagerVita object
// ***************************************************

void GameOverState::Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager)
{
	touch_screen_position_x = front_touch->position.x;
	touch_screen_position_y = front_touch->position.y;
	
	// Detect Touch with menu button
	if (OptionSelected(menu_sprite_, front_touch))
	{
		OnExit(MENU_STATE);
	}
}



// ***************************************************
// Draw function
// Draws all the sprites and text to the screen based on when they should appear
// Takes in a pointer to a SpriteRenderer object and the address of a font object so they can be used
// ***************************************************

void GameOverState::Draw(abfw::SpriteRenderer* sprite_renderer, Font &font)
{
	sprite_renderer->DrawSprite(background_);
	sprite_renderer->DrawSprite(menu_sprite_);
}



// ***************************************************
// On Exit function
// Called when the GameOverState is exited, lets sprite application know to change the state to the new state
// ***************************************************

void GameOverState::OnExit(int new_state)
{
	state_changed_ = true;
	new_state_ = new_state;
}





