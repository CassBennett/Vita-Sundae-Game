#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include "state.h"
#include "Box2D\Box2D.h"
#include "game_object.h"
#include "spoon_manager.h"
#include "scoop_manager.h"
#include "pin.h"
#include <graphics/texture.h>
#include <iostream>
#include <input/vita/touch_input_manager_vita.h>
#include <audio\vita\audio_manager_vita.h>
#include <stdlib.h>
#include <time.h>
#include "joints.h"

using namespace std;

class GameState : public State
{
public:
	// Functions
	GameState();

	// State Functions
	void OnEnter(Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform);
	void Update(float ticks, Touch *front_touch, AudioManagerVita *audio_manager);
	void Draw(SpriteRenderer* sprite_renderer, Font &font);
	void OnExit(int new_state);

	// Initialising Functions
	void SetUpScreen();
	void CreateBodies();
	void CreatePins();
	void SetUserDataForAll();
	void InitialiseSprites();

	// Update functions
	void MoveScoops(Scoops *scoop);
	void DetectCollision();
	void CollisionResponse(GameObject *collision_bodyA, GameObject *collision_bodyB);
	void SetScoopPosition();
	void SetUpScoops();
	void HandleChef();
	void EndGameSequence();
	void UpdateScoopSounds();
	void HandleCherry();

	// General Game Variables/Objects
	b2World* world_;
	Pin *pins_[AMOUNT_OF_PINS];
	GameObject background_;
	GameObject menu_sprite_;
	float score_;
	float frame_rate_;
	Touch *front_touch_;
	Texture *spritesheet_;

	// Audio Variables
	Int32 scoops_falling_index_;
	Int32 scoops_dropped_index_;
	Int32 scoops_grabbed_index_;

	// Managers & Scoop & Spoon IDS
	AudioManagerVita *audio_manager_;
	ScoopManager *scoop_manager_;
	SpoonManager *spoon_manager_;
	int scoop_number_;

	// Joint Objects
	Joints revolute_joint_;
	Joints mouse_joint_;

	// Main Bodies & Sprites
	b2Body *ground_body_;
	b2Body *ceiling_body_;
	b2Body *stand_body_;
	GameObject ground_sprite_;
	GameObject ceiling_sprite_;
	GameObject stand_sprite_;
	b2BodyDef ground_ceiling_def_;
	b2BodyDef stand_def_;
	b2Vec2 ground_position_;
	b2Vec2 ceiling_position_;
	b2Vec2 stand_position_;
	b2Vec2 ground_ceiling_body_dimensions_;
	b2Vec2 stand_dimensions_;
	b2PolygonShape ground_ceiling_shape_;
	b2PolygonShape stand_shape_;
	b2BodyDef body_def_;

	b2Body *plate_body_;
	GameObject plate_sprite_;
	b2Vec2 plate_dimensions_;			
	b2Vec2 plate_initial_position_;	
	b2PolygonShape dynamic_box_;	
	b2FixtureDef fixture_def_;	

	// Edge Variables
	b2Vec2 edge_vertex_1_;
	b2Vec2 edge_vertex_2_;
	b2BodyDef edge_body_def_;
	b2EdgeShape edge;

	// Cherry Variables/Objects
	GameObject finish_box_;
	Vector3 finish_box_position_;
	bool cherry_called_;
	bool can_call_cherry_;
	bool call_cherry_;
	bool cherry_placed_;
	int cherry_timer_;

	// Chef Variables/Objects
	GameObject chef_sprite_;
	int chef_timer_;
	bool chef_called_once_;
	bool chef_called_twice_;
	bool first_call_chef_;
	bool second_call_chef_;

	// Sound Variables
	Int32 pickup_scoop_ID_;
	Int32 landed_ID_;
	Int32 bomb_explosion_ID_;
	Int32 sound_index_;

	// Game End Variables/Objects
	bool game_playing_;
	GameObject line_sprite_;
	bool game_finished_;
	bool display_score_;
	float line_movement_;
	Vector3 line_position_;
	float height_score_;
	float multiplier_;
	float scoop_score_;
	float scoops_lost_;
	float sundae_height;

private:

};

#endif