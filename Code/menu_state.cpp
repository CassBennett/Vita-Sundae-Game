#include "menu_state.h"



// ***************************************************
// MenuState Constructor
// Initialises variables needed when the game is first ran
// ***************************************************

MenuState::MenuState()
{
	state = MENU_STATE;
	state_changed_ = false;
	start_screen_on_ = true;
	start_screen_timer_ = 0.0f;
	initialised_ = false;
}



// ***************************************************
// On Enter function
// Different to state constructor because these are things to be initialised every time this state is entered
// ***************************************************

void MenuState::OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform)
{
	// Initialising
	in_state_ = true;

	if (!initialised_)
	{
		audio_manager->PlayMusic();
		initialised_ = true;
	}

	state = MENU_STATE;
	state_changed_ = false;
	new_state_ = MENU_STATE;

	// Game Objects
	background_.set_width(DISPLAY_WIDTH);
	background_.set_height(DISPLAY_HEIGHT);
	background_.GiveTexture(spritesheet);
	background_.TextureSettings(Vector2(0.5, 0.3), 0.25, 0.15);
	background_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	select_game_sprite_.set_width(70.0f);
	select_game_sprite_.set_height(150.0f);
	select_game_sprite_.GiveTexture(spritesheet);
	select_game_sprite_.TextureSettings(Vector2(0.75, 0.15), 0.05, 0.1);
	select_game_sprite_.set_position(Vector3(450.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	select_options_sprite_.set_width(70.0f);
	select_options_sprite_.set_height(150.0f);
	select_options_sprite_.GiveTexture(spritesheet);
	select_options_sprite_.TextureSettings(Vector2(0.8, 0.15), 0.05, 0.1);
	select_options_sprite_.set_position(Vector3(350.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	select_tutorial_sprite_.set_width(70.0f);
	select_tutorial_sprite_.set_height(150.0f);
	select_tutorial_sprite_.GiveTexture(spritesheet);
	select_tutorial_sprite_.TextureSettings(Vector2(0.9, 0.15), 0.05, 0.1);
	select_tutorial_sprite_.set_position(Vector3(250.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	start_screen_sprite_.set_width(DISPLAY_WIDTH);
	start_screen_sprite_.set_height(DISPLAY_HEIGHT);
	start_screen_sprite_.GiveTexture(spritesheet);
	start_screen_sprite_.TextureSettings(Vector2(0.5f, 0.15), 0.25, 0.15);
	start_screen_sprite_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	// Audio Initialisation
	sound_index_ = -1.0f;
	startup_noise_ID_ = audio_manager->LoadSample("startup_noise.wav", *platform);
	click_ID_ = audio_manager->LoadSample("click_noise.wav", *platform);
}



// ***************************************************
// Update function
// Gets the touch screen position and detects if player has selected buttons
// Takes in float for frame rate, Touch object and AudioManagerVita object
// ***************************************************

void MenuState::Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager)
{
	// Display Splash screen only for a certain amount of time
	if (start_screen_on_)
	{
		start_screen_timer_ = 200;
		display_splash_screen_ = true;
		start_screen_on_ = false;
	}

	if (start_screen_timer_ >= 0)
	{
		start_screen_timer_--;
	}

	if (start_screen_timer_ == 0)
	{
		sound_index_ = audio_manager->PlaySample(startup_noise_ID_, false);
		display_splash_screen_ = false;
	}

	// Play noise when screen is tapped
	if ((front_touch->type == TT_NEW) && (sound_FX_on_))
	{
		sound_index_ = audio_manager->PlaySample(click_ID_, false);
	}
	
	// Detect if the player has selected an option and exit the state based on the option
	if (front_touch->type == TT_RELEASED)
	{
		if (OptionSelected(select_game_sprite_, front_touch))
		{
			OnExit(GAME_STATE);
		}
		else if (OptionSelected(select_options_sprite_, front_touch))
		{
			OnExit(OPTIONS_STATE);
		}
		else if (OptionSelected(select_tutorial_sprite_, front_touch))
		{
			OnExit(INSTRUCTIONS_STATE);
		}
	}
}



// ***************************************************
// Draw function
// Draws all the sprites and text to the screen based on when they should appear
// Takes in a pointer to a SpriteRenderer object and the address of a font object so they can be used
// ***************************************************

void MenuState::Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font)
{
	sprite_renderer_->DrawSprite(background_);
	sprite_renderer_->DrawSprite(select_game_sprite_);
	sprite_renderer_->DrawSprite(select_options_sprite_);
	sprite_renderer_->DrawSprite(select_tutorial_sprite_);

	if (display_splash_screen_)
	{
		sprite_renderer_->DrawSprite(start_screen_sprite_);
	}
}



// ***************************************************
// On Exit function
// Called when the MenuState is exited, lets sprite application know to change the state to the new state
// ***************************************************

void MenuState::OnExit(int new_state)
{
	in_state_ = false;
	state_changed_ = true;
	new_state_ = new_state;
	state_changed_ = false;
}



