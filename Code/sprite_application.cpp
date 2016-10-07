#include "sprite_application.h"
#include <graphics/sprite_renderer.h>
#include <system/platform.h>
#include <system/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
#include <iostream>
#include "box2d_helpers.h"
#include "definitions.h"

SpriteApplication::SpriteApplication(Platform* platform) :
	Application(platform),
	sprite_renderer_(NULL),
	controller_manager_(SCE_CTRL_MODE_DIGITALANALOG)
{
}

SpriteApplication::~SpriteApplication()
{
}



// ***************************************************
// Init function
// Initialises everything for the application such as the Box2d world, gravity, and bodies involved
// ***************************************************

void SpriteApplication::Init()
{
	// Load in font for the game
	bool font_loaded = font_.Load("century_gothic", *platform_);
	if(!font_loaded)
	{
		std::cout << "Font failed to load." << std::endl;
		exit(-1);
	}

	// Initialise initial background as game loads
	background_.set_width(DISPLAY_WIDTH);
	background_.set_height(DISPLAY_HEIGHT);
	background_.set_colour(0xff000000);
	background_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	// Sets current state to the Menu State
	current_state_ = &menu_state_;
	entered_ = false;

	sprite_renderer_ = platform_->CreateSpriteRenderer();

	// Load spritesheet
	spritesheet_ = LoadTextureFromPNG("vita_spritesheet.png");

	// Enable front touch screen for the game
	touch_manager_.EnablePanel(SCE_TOUCH_PORT_FRONT);
	
	// Audio Initialisation
	audio_manager_ = new AudioManagerVita();
	audio_manager_->LoadMusic("background_music.wav", *platform_);
}



// ***************************************************
// Load Texture From PNG function
// Takes in the filename of the texture to be loaded from the bin folder and returns the texture 
// ***************************************************

Texture* SpriteApplication::LoadTextureFromPNG(const char* filename)
{
	png_loader_.Load(filename, *platform_, image_data_);
Texture* new_texture;

	if (image_data_.image() != NULL)	// Checks to see if the image has loaded properly
	{
		new_texture = platform_->CreateTexture(image_data_);
		return new_texture;
	}
	else if (image_data_.image() == NULL)	// Checks to see if the image has loaded properly
	{
		return NULL;
	}
}



// ***************************************************
// Clean Up function
// Deletes all objects from the game
// ***************************************************

void SpriteApplication::CleanUp()
{
	audio_manager_->UnloadAllSamples();
	audio_manager_->UnloadMusic();
	delete audio_manager_;
	audio_manager_  = NULL;
	
	// free up the sprite renderer
	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}



// ***************************************************
// Update function
// Updates the main managers such as the controller manager and touch manager, and updates the current state
// ***************************************************

bool SpriteApplication::Update(float ticks)
{
	// Updates for game
	controller_manager_.Update();
	touch_manager_.Update();
	front_touch_ = touch_manager_.GetTouch(SCE_TOUCH_PORT_FRONT, 0);
	DetectStateChange();

	// Only calls OnEnter for each state when it's entered once and passes in the appropriate objects/information
	if (!entered_)
	{
		current_state_->OnEnter(spritesheet_, audio_manager_, platform_);
		entered_ = true;
	}

	current_state_->Update(ticks, &front_touch_, audio_manager_);
	DetectGameChanges();

	return true;
}



// ***************************************************
// Detect Game Changes function
// Detects when bools within OptionsState change and then changes the same bool in GameState appropriately
// ***************************************************

void SpriteApplication::DetectGameChanges()
{
	// Changes GameState difficulty when OptionsState difficulty changes
	if (options_state_.hard_difficulty_on_)
	{
		game_state_.hard_difficulty_on_ = true;
	}
	else if (!options_state_.hard_difficulty_on_)
	{
		game_state_.hard_difficulty_on_ = false;
	}

	// Changes GameState FX on when OptionsState FX changes
	if (options_state_.sound_FX_on_)
	{
		game_state_.sound_FX_on_ = true;
		menu_state_.sound_FX_on_ = true;
	}
	else if (!options_state_.sound_FX_on_)
	{
		game_state_.sound_FX_on_ = false;
		menu_state_.sound_FX_on_ = false;
	}
}



// ***************************************************
// Render function
// Uses the sprite renderer to render sprites and text to the screen
// ***************************************************

void SpriteApplication::Render()
{
	// set up sprite renderer for drawing
	sprite_renderer_->Begin();

	sprite_renderer_->DrawSprite(background_);

	if (current_state_->in_state_)
	{
		current_state_->Draw(sprite_renderer_, font_);
	}

	sprite_renderer_->End();
}



// ***************************************************
// Detect State Change function
// Changes the State of the game if the new state of the game is different to the original
// ***************************************************

void SpriteApplication::DetectStateChange()
{
	previous_state_ = current_state_;

	// Checks when the new state and original state don't match, therefore the state has changed in the OnExit function
	if (current_state_->new_state_ != current_state_->state)
	{
		changed_ = true;
	}

	// Change the current state to point to the new state and set the previous state's new state variable back to its original
	if (changed_ == true)
	{
		if (current_state_->new_state_ == State::GAME_STATE)
		{
			current_state_ = &game_state_;
		}
		else if (current_state_->new_state_ == State::MENU_STATE)
		{
			current_state_ = &menu_state_;
		}
		else if (current_state_->new_state_ == State::OPTIONS_STATE)
		{
			current_state_ = &options_state_;
		}
		else if (current_state_->new_state_ == State::GAMEOVER_STATE)
		{
			current_state_ = &gameover_state_;
		}
		else if (current_state_->new_state_ == State::INSTRUCTIONS_STATE)
		{
			current_state_ = &instructions_state_;
		}

		entered_ = false;
		previous_state_->new_state_ = previous_state_->state;
		changed_ = false;
	}
}