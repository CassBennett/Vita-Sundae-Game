#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <graphics\sprite.h>
#include <Box2D/Box2D.h>
#include <maths\vector3.h>
#include <system/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
#include "Box2D\Box2D.h"

using namespace abfw;

class GameObject : public Sprite
{
public:
	// Functions
	GameObject();
	void GiveID(int ID_number);
	void RotateSpriteToBody(b2Body *body, float rate);
	float GetX();
	float GetY();
	void ResizeSprite(float new_width, float new_height);

	// Variables
	Vector3 GetPosition();
	int ID_;

	// Game Object type enumerators & set type functions
	enum ObjectType { INIT, GROUND, CEILING, STAND, SCOOP, SPOON, PLATE, };
	enum ScoopType { NORMAL_SCOOP, BOMB_SCOOP, CHERRY };
	void SetObjectType(ObjectType type);
	void SetScoopType(ScoopType type);
	ObjectType type_; 
	ScoopType scoop_type_;

	// Functions & Variables that handle textures
	void GiveTexture(Texture *sprite_sheet);
	void TextureSettings(Vector2 uv_position, float uv_width, float uv_height);
	void ChangeTexture(float uv_x, float uv_y);
	Texture *sprite_texture_;

	// Functions & Variables that handle animation
	void SetAnimationSettings();
	void Animate();
	int game_frame_count_;	// Number of game update frames to wait between animation frame changes number of animation frames
	int num_frames_;		
	float uv_height_;
	Vector2 uv_position_;
	Vector2 unchanged_uv_position_;
	int animation_counter_;		// animation counter
	int animation_timer_;
};

#endif