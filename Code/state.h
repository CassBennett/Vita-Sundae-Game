#ifndef _STATE_H
#define _STATE_H

#include <system/application.h>
#include "graphics\sprite_renderer.h"
#include <input/vita/touch_input_manager_vita.h>
#include <graphics/texture.h>
#include <graphics/font.h>
#include <audio/vita/audio_manager_vita.h>
#include "game_object.h"

using namespace abfw;

class State
{
public:
	// Functions
	State();
	virtual void Draw(SpriteRenderer* sprite_renderer, Font &font) = 0;
	virtual void OnEnter(Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform) = 0;
	virtual void Update(float ticks, Touch *front_touch, AudioManagerVita *audio_manager) = 0;
	virtual void OnExit(int new_state) = 0;
	bool OptionSelected(GameObject option_box, Touch *touch);

	// Detect Touch Variables
	float touch_screen_position_x;
	float touch_screen_position_y;
	Vector3 box_position_;
	float half_width_;
	float half_height_;

	// State variables
	enum CurrentState { MENU_STATE, GAME_STATE, OPTIONS_STATE, GAMEOVER_STATE, GAMEWON_STATE, INSTRUCTIONS_STATE };
	CurrentState state;
	bool state_changed_;
	bool start_screen_on_;
	bool in_state_;
	int new_state_;

	// Game Option Variables
	bool hard_difficulty_on_;
	bool sound_FX_on_;
private:

};

#endif