#ifndef _INSTRUCTIONS_STATE_H
#define _INSTRUCTIONS_STATE_H

#include <audio/vita/audio_manager_vita.h>
#include "state.h"
#include "graphics\sprite_renderer.h"
#include "maths\vector3.h"
#include "maths\vector2.h"
#include "game_object.h"
#include "definitions.h"

using namespace std;

class InstructionsState : public State
{
public:
	// Functions
	InstructionsState();
	void OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform);
	void Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager);
	void Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font);
	void OnExit(int new_state);
	void TouchResponse();

	// General Variables/Objects
	Touch *front_touch_;
	
	bool first_page_on_;
	bool second_page_on_;

	// Game Objects
	GameObject background_;
	GameObject first_page_;
	GameObject second_page_;
	GameObject menu_button_;
	GameObject next_button_;

	// Audio
	Int32 sound_index_;
	Int32 click_ID_;

private:

};

#endif