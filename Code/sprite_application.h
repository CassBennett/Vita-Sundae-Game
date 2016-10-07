#ifndef _SPRITE_APPLICATION_H
#define _SPRITE_APPLICATION_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <input/vita/controller_input_manager_vita.h>
#include <input/vita/touch_input_manager_vita.h>
#include <Box2D/Box2D.h>
#include <system/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
#include "game_object.h"
#include "state.h"
#include "game_state.h"
#include "menu_state.h"
#include "options_state.h"
#include "gameover_state.h"
#include "instructions_state.h"

namespace abfw
{
	class Platform;
	class Texture;
}

using namespace std;
using namespace abfw;

class SpriteApplication : public Application
{
public:
	// Functions
	SpriteApplication(Platform* platform);
	~SpriteApplication();
	void Init();
	void CleanUp();
	bool Update(float ticks);
	void Render();
	void DetectStateChange();
	void DetectGameChanges();
	Texture* LoadTextureFromPNG(const char* filename);

private:
	ControllerInputManagerVita controller_manager_;
	Font font_;
	SpriteRenderer* sprite_renderer_;
	TouchInputManagerVita touch_manager_;
	Touch front_touch_;

	// Texture Variables/Objects
	PNGLoader png_loader_;
	ImageData image_data_;
	Texture* spritesheet_;

	// State Objects
	MenuState menu_state_;
	GameState game_state_;
	OptionsState options_state_;
	GameOverState gameover_state_;
	InstructionsState instructions_state_;
	State* current_state_;
	State* previous_state_;
	bool entered_;
	bool changed_;

	// Background between switching states
	GameObject background_;

	// Audio Objects
	AudioManagerVita* audio_manager_;
	VolumeInfo volume_info;

};

#endif // _SPRITE_APPLICATION_H