#ifndef _GAMEOVER_STATE_H
#define _GAMEOVER_STATE_H

#include "state.h"
#include "game_object.h"
#include "definitions.h"
#include "maths\vector3.h"

using namespace std;

class GameOverState : public State
{
public:
	// Functions
	GameOverState();
	void OnEnter(abfw::Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform);
	void Update(float ticks, abfw::Touch *front_touch, AudioManagerVita *audio_manager);
	void Draw(abfw::SpriteRenderer* sprite_renderer_, Font &font);
	void OnExit(int new_state);

	// Game Objects
	GameObject background_;
	GameObject menu_sprite_;

private:

};

#endif