#ifndef _OPTIONS_STATE_H
#define _OPTIONS_STATE_H

#include <audio/vita/audio_manager_vita.h>
#include "state.h"
#include "graphics\sprite_renderer.h"
#include "maths\vector3.h"
#include "maths\vector2.h"
#include "game_object.h"
#include "definitions.h"

class OptionsState : public State
{
public:
	// Functions
	OptionsState();
	void OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform);
	void Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager);
	void Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font);
	void OnExit(int new_state);
	void TouchResponse();

	// General Variables/Objects
	Touch *front_touch_;

	// GameObjects
	GameObject background_;
	GameObject lower_sound_;
	GameObject higher_sound_;
	GameObject toggle_difficulty_;
	GameObject toggle_FX_;
	GameObject toggle_music_;
	GameObject go_back_;

	// Booleans for options
	bool music_on_;
	bool sound_lower_;
	bool sound_higher_;

	// Audio Variables
	AudioManagerVita *audio_manager_;
	VolumeInfo volume_info;
	float sample_volume_;
	float sample_pan_;
	Int32 sound_index_;
	Int32 click_ID_;

private:

};

#endif