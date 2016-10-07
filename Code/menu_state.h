#ifndef _MENU_STATE_H
#define _MENU_STATE_H

#include "state.h"
#include "graphics\sprite_renderer.h"
#include "maths\vector3.h"
#include "maths\vector2.h"
#include "game_object.h"
#include "definitions.h"



class MenuState : public State
{
public:
	// Functions
	MenuState();
	void OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform);
	void Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager);
	void Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font);
	void OnExit(int new_state);

	// General Variables/Objects
	int start_screen_timer_;
	Int32 sound_index_;
	Int32 click_ID_;
	Int32 startup_noise_ID_;
	bool display_splash_screen_;
	bool initialised_;

	// Game Objects
	GameObject background_;
	GameObject select_game_sprite_;
	GameObject select_options_sprite_;
	GameObject select_tutorial_sprite_;
	GameObject start_screen_sprite_;
	
private:

};

#endif