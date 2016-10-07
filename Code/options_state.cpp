#include "options_state.h"



// ***************************************************
// OptionsState Constructor
// Initialises variables needed when the game is first ran
// ***************************************************

OptionsState::OptionsState()
{
	// These are changed within the game therefore they are initialised here 
	music_on_ = true;
	sound_FX_on_ = true;
	sound_lower_ = false;
	sound_higher_ = false;
	hard_difficulty_on_ = false;
	sample_pan_ = 0.0f;
	sample_volume_ = 0.5f;
}



// ***************************************************
// On Enter function
// Different to state constructor because these are things to be initialised every time this state is entered
// ***************************************************

void OptionsState::OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform)
{
	in_state_ = true;
	state = OPTIONS_STATE;
	state_changed_ = false;

	// Audio Initialisation
	sound_index_ = -1.0f;
	click_ID_ = audio_manager->LoadSample("click_noise.wav", *platform);

	// Game Objects
	background_.set_width(DISPLAY_WIDTH);
	background_.set_height(DISPLAY_HEIGHT);
	background_.GiveTexture(spritesheet);
	background_.TextureSettings(Vector2(0.5, 0.45), 0.25, 0.15);
	background_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	lower_sound_.set_width(70.0f);
	lower_sound_.set_height(70.0f);
	lower_sound_.GiveTexture(spritesheet);
	lower_sound_.TextureSettings(Vector2(0.8, 0.0), 0.05, 0.05);
	lower_sound_.set_position(Vector3(600.0f, HALF_DISPLAY_HEIGHT - 40.0f, 0.0f));

	higher_sound_.set_width(70.0f);
	higher_sound_.set_height(70.0f);
	higher_sound_.GiveTexture(spritesheet);
	higher_sound_.TextureSettings(Vector2(0.85, 0.0), 0.05, 0.05);
	higher_sound_.set_position(Vector3(600.0f, HALF_DISPLAY_HEIGHT + 40.0f, 0.0f));

	toggle_difficulty_.set_width(70.0f);
	toggle_difficulty_.set_height(120.0f);
	toggle_difficulty_.GiveTexture(spritesheet);
	toggle_difficulty_.TextureSettings(Vector2(0.85, 0.15), 0.05, 0.1);
	toggle_difficulty_.set_position(Vector3(500.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	toggle_FX_.set_width(70.0f);
	toggle_FX_.set_height(120.0f);
	toggle_FX_.GiveTexture(spritesheet);
	toggle_FX_.TextureSettings(Vector2(0.8, 0.25), 0.05, 0.1);
	toggle_FX_.set_position(Vector3(400.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	toggle_music_.set_width(70.0f);
	toggle_music_.set_height(120.0f);
	toggle_music_.GiveTexture(spritesheet);
	toggle_music_.TextureSettings(Vector2(0.85, 0.05), 0.05, 0.1);
	toggle_music_.set_position(Vector3(300.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	go_back_.set_width(80.0f);
	go_back_.set_height(80.0f);
	go_back_.GiveTexture(spritesheet);
	go_back_.TextureSettings(Vector2(0.75, 0.0), 0.05, 0.05);
	go_back_.set_position(Vector3(95.0f, HALF_DISPLAY_HEIGHT, 0.0f));
}



// ***************************************************
// Update function
// Gets the touch screen position and detects if player has selected buttons
// Takes in float for frame rate, Touch object and AudioManagerVita object
// ***************************************************

void OptionsState::Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager)
{
	audio_manager_ = audio_manager;
	front_touch_ = front_touch;

	if (front_touch->type == TT_NEW)
	{
		if (sound_FX_on_)
		{
			sound_index_ = audio_manager->PlaySample(click_ID_, false);
		}
		TouchResponse();
	}

	// Set Audio Data
	volume_info.volume = sample_volume_;
	volume_info.pan = sample_pan_;
	audio_manager_->SetMusicVolumeInfo(volume_info);
}



// ***************************************************
// Draw function
// Draws all the sprites and text to the screen based on when they should appear
// Takes in a pointer to a SpriteRenderer object and the address of a font object so they can be used
// ***************************************************

void OptionsState::Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font)
{
	// Draw all backgrounds and buttons
	sprite_renderer_->DrawSprite(background_);
	sprite_renderer_->DrawSprite(lower_sound_);
	sprite_renderer_->DrawSprite(higher_sound_);
	sprite_renderer_->DrawSprite(toggle_difficulty_);
	sprite_renderer_->DrawSprite(toggle_FX_);
	sprite_renderer_->DrawSprite(toggle_music_);
	sprite_renderer_->DrawSprite(go_back_);

	font.RenderText(sprite_renderer_, Vector3(665.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xFF000000, TJ_CENTRE, "Lower and Higher sound!");
	font.RenderText(sprite_renderer_, Vector3(565.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xFF000000, TJ_CENTRE, "Toggle the difficulty!");
	font.RenderText(sprite_renderer_, Vector3(465.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xFF000000, TJ_CENTRE, "Toggle the sound effects!");
	font.RenderText(sprite_renderer_, Vector3(365.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xFF000000, TJ_CENTRE, "Toggle the music!");
}



// ***************************************************
// On Exit function
// Called when the OptionsState is exited, lets sprite application know to change the state to the new state
// ***************************************************

void OptionsState::OnExit(int new_state)
{
	in_state_ = false;
	state_changed_ = true;
	new_state_ = new_state;
}



// ***************************************************
// Touch Response function
// Detects which button has been selected using the Touch Screen touch position
// Carries out the appropriate response when a button is selected
// ***************************************************

void OptionsState::TouchResponse()
{
	// detect touch with the sprites here
	if (OptionSelected(lower_sound_, front_touch_))
	{
		// Lower Sound
		if (sample_volume_ > 0)
		{
			sample_volume_ -= 0.1f;
			sound_lower_ = true;
		}
	}

	else if (OptionSelected(higher_sound_, front_touch_))
	{
		// Higher Sound
		if (sample_volume_ < 1)
		{
			sample_volume_ += 0.1f;
			sound_higher_ = true;
		}
	}

	else if (OptionSelected(toggle_difficulty_, front_touch_))
	{
		// Turn on Hard Difficulty
		if (!hard_difficulty_on_)
		{
			toggle_difficulty_.set_uv_position(Vector2(0.75, 0.05f));
			hard_difficulty_on_ = true;
		}

		// Turn off Hard Difficulty
		else if (hard_difficulty_on_)
		{
			toggle_difficulty_.set_uv_position(Vector2(0.85, 0.15f));
			hard_difficulty_on_ = false;
		}
	}

	else if (OptionSelected(toggle_FX_, front_touch_))
	{
		// Turn Sound FX off
		if (sound_FX_on_ == true)
		{
			toggle_FX_.set_uv_position(Vector2(0.75, 0.25f));
			sound_FX_on_ = false;
		}

		// Turn Sound FX on
		else if (sound_FX_on_ == false)
		{
			toggle_FX_.set_uv_position(Vector2(0.8, 0.25f));
			sound_FX_on_ = true;
		}
	}

	else if (OptionSelected(toggle_music_, front_touch_))
	{
		// Turn Music off
		if (music_on_ == true)
		{
			toggle_music_.set_uv_position(Vector2(0.8, 0.05f));
			audio_manager_->StopMusic();
			music_on_ = false;
		}

		// Turn Music On
		else if (music_on_ == false)
		{
			toggle_music_.set_uv_position(Vector2(0.85, 0.05f));
			audio_manager_->PlayMusic();
			music_on_ = true;
		}
	}

	else if (OptionSelected(go_back_, front_touch_))
	{
		// Exit to menu
		OnExit(State::MENU_STATE);
	}
}



