#include "instructions_state.h"



// ***************************************************
// InstructionsState Constructor
// Initialises variables needed when the game is first ran
// ***************************************************

InstructionsState::InstructionsState()
{
	in_state_ = false;
	state = INSTRUCTIONS_STATE;
	state_changed_ = false;
}



// ***************************************************
// On Enter function
// Different to state constructor because these are things to be initialised every time this state is entered
// ***************************************************

void InstructionsState::OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform)
{
	in_state_ = true;
	state = INSTRUCTIONS_STATE;
	state_changed_ = false;
	new_state_ = INSTRUCTIONS_STATE;

	// Audio Initialisation
	sound_index_ = -1.0;
	click_ID_ = audio_manager->LoadSample("click_noise.wav", *platform);

	// Set up sprites

	background_.set_width(DISPLAY_WIDTH);
	background_.set_height(DISPLAY_HEIGHT);
	background_.GiveTexture(spritesheet);
	background_.TextureSettings(Vector2(0.5, 0.75), 0.25, 0.15);
	background_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	menu_button_.set_width(70.0f);
	menu_button_.set_height(150.0f);
	menu_button_.GiveTexture(spritesheet);
	menu_button_.TextureSettings(Vector2(0.9f, 0.05f), 0.05f, 0.1f);
	menu_button_.set_position(Vector3(80.0f, 445.0f, 0.0f));

	first_page_.set_width(DISPLAY_WIDTH);
	first_page_.set_height(DISPLAY_HEIGHT);
	first_page_.GiveTexture(spritesheet);
	first_page_.TextureSettings(Vector2(0.75f, 0.45f), 0.25f, 0.15f);
	first_page_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	second_page_.set_width(DISPLAY_WIDTH);
	second_page_.set_height(DISPLAY_HEIGHT);
	second_page_.GiveTexture(spritesheet);
	second_page_.TextureSettings(Vector2(0.75f, 0.6f), 0.25f, 0.15f);
	second_page_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	next_button_.set_width(70.0f);
	next_button_.set_height(70.0f);
	next_button_.GiveTexture(spritesheet);
	next_button_.TextureSettings(Vector2(0.85f, 0.0f), 0.05f, 0.05f);
	next_button_.set_position(Vector3(80.0f, 80.0f, 0.0f));

	first_page_on_ = true;
	second_page_on_ = false;
}



// ***************************************************
// Update function
// Gets the touch screen position and detects if player has selected buttons
// Takes in float for frame rate, Touch object and AudioManagerVita object
// ***************************************************

void InstructionsState::Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager)
{
	//touch_screen_position_x = front_touch->position.x;
	//touch_screen_position_y = front_touch->position.y;
	
	// Detect touch with button sprites
	if ((front_touch->type == TT_NEW) && (sound_FX_on_))
	{
		sound_index_ = audio_manager->PlaySample(click_ID_, false);
	}

	if (OptionSelected(menu_button_, front_touch))
	{
		OnExit(MENU_STATE);
	}

	// Changes page that should be displayed
	if (OptionSelected(next_button_, front_touch))
	{
		first_page_on_ = false;
		second_page_on_ = true;
	}
}



// ***************************************************
// Draw function
// Draws all the sprites and text to the screen based on when they should appear
// Takes in a pointer to a SpriteRenderer object and the address of a font object so they can be used
// ***************************************************

void InstructionsState::Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font)
{
	// Back to front 
	sprite_renderer_->DrawSprite(background_);

	if (first_page_on_)
	{
		sprite_renderer_->DrawSprite(first_page_);
		sprite_renderer_->DrawSprite(next_button_);
		font.RenderText(sprite_renderer_, Vector3(680.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xFF000000, TJ_CENTRE, "The aim of the game is to create");
		font.RenderText(sprite_renderer_, Vector3(660.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xFF000000, TJ_CENTRE, "the tallest icecream!");

		font.RenderText(sprite_renderer_, Vector3(600.0f, 380.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "Grab scoops from spoons");
		font.RenderText(sprite_renderer_, Vector3(580.0f, 380.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "& drop them");
		font.RenderText(sprite_renderer_, Vector3(560.0f, 380.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "onto the plate!");

		font.RenderText(sprite_renderer_, Vector3(410.0f, 160.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "Try to balance the");
		font.RenderText(sprite_renderer_, Vector3(390.0f, 160.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "scoops or the plate");
		font.RenderText(sprite_renderer_, Vector3(370.0f, 160.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "will sway & drop");
		font.RenderText(sprite_renderer_, Vector3(350.0f, 160.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "ice cream!");

		font.RenderText(sprite_renderer_, Vector3(180.0f, 380.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "Drop too many &");
		font.RenderText(sprite_renderer_, Vector3(160.0f, 380.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "you'll make the");
		font.RenderText(sprite_renderer_, Vector3(140.0f, 380.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "chef mad!");
	}

	else if (second_page_on_)
	{
		sprite_renderer_->DrawSprite(second_page_);
		sprite_renderer_->DrawSprite(menu_button_);

		font.RenderText(sprite_renderer_, Vector3(640.0f, 190.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "Watch out for bombs");
		font.RenderText(sprite_renderer_, Vector3(620.0f, 190.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "disguised as scoops!");

		font.RenderText(sprite_renderer_, Vector3(470.0f, 390.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "When you think its as");
		font.RenderText(sprite_renderer_, Vector3(450.0f, 390.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "high as can be, click");
		font.RenderText(sprite_renderer_, Vector3(430.0f, 390.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "the cherry button &");
		font.RenderText(sprite_renderer_, Vector3(410.0f, 390.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "drop the cherry on top!");

		font.RenderText(sprite_renderer_, Vector3(290.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "Your score is based on");
		font.RenderText(sprite_renderer_, Vector3(270.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "how many scoops you have,");
		font.RenderText(sprite_renderer_, Vector3(250.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "how many you dropped,");
		font.RenderText(sprite_renderer_, Vector3(230.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "& the height above the plate");
		font.RenderText(sprite_renderer_, Vector3(210.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "the cherry is!");

		font.RenderText(sprite_renderer_, Vector3(160.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "Drop too many, let scoops");
		font.RenderText(sprite_renderer_, Vector3(140.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "touch fallen scoops,");
		font.RenderText(sprite_renderer_, Vector3(120.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "or let the cherry fall on");
		font.RenderText(sprite_renderer_, Vector3(100.0f, 220.0f, 0.0f), 0.9f, 0xFF000000, TJ_CENTRE, "the ground, it's game over!");
	}
}



// ***************************************************
// On Exit function
// Called when the InstructionsState is exited, lets sprite application know to change the state to the new state
// ***************************************************

void InstructionsState::OnExit(int new_state)
{
	state_changed_ = true;
	new_state_ = new_state;
}



