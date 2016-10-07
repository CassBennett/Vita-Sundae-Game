#include "game_state.h"




// ***************************************************
// GameState Constructor
// Initialises variables needed when the game is first ran
// ***************************************************

GameState::GameState()
{
	state = GAME_STATE;
	new_state_ = GAME_STATE;
	hard_difficulty_on_ = false; 
}




// ***************************************************
// On Enter function
// Different to state constructor because these are things to be initialised every time this state is entered
// ***************************************************

void GameState::OnEnter(Texture *spritesheet, AudioManagerVita *audio_manager, Platform *platform)
{
	in_state_ = true;
	srand (time(NULL));
	
	state = GAME_STATE;
	new_state_ = GAME_STATE;

	// Set gravity
	b2Vec2 gravity(-10.0f, 0.0f);

	// Create objects and managers
	world_ = new b2World(gravity);
	SetUpScreen();
	scoop_manager_ = new ScoopManager(world_);
	spoon_manager_ = new SpoonManager(world_, spritesheet);

	// Get Spritesheet
	spritesheet_ = spritesheet;
	
	// Initialise sound variables
	sound_index_ = -1.0;
	bomb_explosion_ID_ = audio_manager->LoadSample("bomb_noise.wav", *platform);
	pickup_scoop_ID_ = audio_manager->LoadSample("pickup_noise.wav", *platform);
	landed_ID_ = audio_manager->LoadSample("splat_noise.wav", *platform);

	// Create Bodies
	CreateBodies();
	SetUserDataForAll();
	
	// Set up sprites
	InitialiseSprites();

	// Initialise Chef and Cherry variables
	first_call_chef_ = false;
	second_call_chef_ = false; 
	chef_called_once_ = false;
	chef_called_twice_ = false;
	chef_timer_ = -5;
	cherry_called_ = false;
	can_call_cherry_ = false;
	call_cherry_ = false;
	cherry_placed_ = false;
	cherry_timer_ = 0;

	// Initialise end of game variables
	game_playing_ = true;
	game_finished_ = false;
	display_score_ = false;
	height_score_ = 0;
	scoop_score_ = 0;
	line_movement_ = 0.0f;
	height_score_ = 0.0f;
	multiplier_ = 0.0f;
	scoop_score_ = 0.0f;
	scoops_lost_ = 0.0f;
	score_ = 0;
	scoop_number_ = 0;
	sundae_height = 0.0f;
}



// ***************************************************
// Update function
// Updates the box 2d world
// Updates all the objects and managers in the game, also handles stage of level the game is in 
// Takes in float for frame rate, Touch object and AudioManagerVita object
// ***************************************************

void GameState::Update(float ticks, Touch *front_touch,  AudioManagerVita *audio_manager)
{
	front_touch_ = front_touch;
	audio_manager_ = audio_manager;

	// calculate the frame rate
	frame_rate_ = 1.0f / ticks;

	// Update World
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(
	timeStep,
	velocityIterations,
	positionIterations);

	// Update Game
	DetectCollision();
	scoop_manager_->Update(world_);
	spoon_manager_->Update(scoop_manager_);
	SetUpScoops();
	HandleChef();
	UpdateScoopSounds();
	HandleCherry();

	// Move Scoops
	if (game_playing_)
	{
		for (int i = 0; i < scoop_manager_->current_scoop_amount_; i++)
		{
			MoveScoops(scoop_manager_->scoop[i]);
		}
	}
	
	// Calculate Score
	scoops_lost_ = scoop_manager_->scoops_fallen_;
	multiplier_ = 100 * (scoop_score_ - scoops_lost_);
	score_ = height_score_ * multiplier_;

	// Determine Game Stage
	if (cherry_placed_)
	{
		EndGameSequence();
	}
	if (hard_difficulty_on_)
	{
		for (int i = 0; i < AMOUNT_OF_PINS; i++)
		{
			pins_[i]->Update();
		}
	}

	// Update sprites with dynamic plate body
	b2Vec2 box1_bodypos = plate_body_->GetPosition();
	plate_sprite_.set_position(Vector3(BOX2D_GFX_POS_X(box1_bodypos.x), BOX2D_GFX_POS_Y(box1_bodypos.y), 0.0f));

	float plate_angle = -(plate_body_->GetAngle());
	plate_sprite_.set_rotation(plate_angle);
}



// ***************************************************
// Draw function
// Draws all the sprites and text to the screen based on when they should appear
// Takes in a pointer to a SpriteRenderer object and the address of a font object so they can be used
// ***************************************************

void GameState::Draw(SpriteRenderer* sprite_renderer, Font &font)
{
	// Draw all sprites here, back to front of screen
	sprite_renderer->DrawSprite(background_);
	sprite_renderer->DrawSprite(ground_sprite_);
	sprite_renderer->DrawSprite(ceiling_sprite_);
	sprite_renderer->DrawSprite(stand_sprite_);

	for (int i = 0; i < scoop_manager_->current_scoop_amount_; i++)
	{
		sprite_renderer->DrawSprite(scoop_manager_->scoop[i]->scoop_sprite_);
	}
	for (int i = 0; i < MAX_SPOONS; i++)
	{
		sprite_renderer->DrawSprite(spoon_manager_->spoons[i]->spoon_sprite_);
	}

	sprite_renderer->DrawSprite(spoon_manager_->conveyor_belt_1_);
	 sprite_renderer->DrawSprite(spoon_manager_->conveyor_belt_2_);

	if (hard_difficulty_on_)
	{
		for (int i = 0; i < AMOUNT_OF_PINS; i++)
		{
			sprite_renderer->DrawSprite(pins_[i]->pin_sprite_);
		}
	}

	if (can_call_cherry_)
	{
		sprite_renderer->DrawSprite(finish_box_);
	}

	if (cherry_called_)
	{
		sprite_renderer->DrawSprite(scoop_manager_->cherry_->scoop_sprite_);
	}

	sprite_renderer->DrawSprite(plate_sprite_);
	sprite_renderer->DrawSprite(line_sprite_);
	sprite_renderer->DrawSprite(chef_sprite_);

	font.RenderText(sprite_renderer, Vector3(10.0f, 480.0f, -0.9f), 1.0f, 0xff000000, TJ_LEFT, "FPS: %.1f", frame_rate_);

	if (display_score_)
	{
		sprite_renderer->DrawSprite(menu_sprite_);
		font.RenderText(sprite_renderer, Vector3(950.0f, 280.0f, 0.0f), 0.9f, 0xff000000, TJ_CENTRE, "Scoop amount:  %.0f", scoop_score_);
		font.RenderText(sprite_renderer, Vector3(900.0f, HALF_DISPLAY_HEIGHT, 0.0f), 0.9f, 0xff000000, TJ_CENTRE, "Scoops Fallen:  %.0f", scoops_lost_);
		font.RenderText(sprite_renderer, Vector3(850.0f, HALF_DISPLAY_HEIGHT, 0.0f), 0.9f, 0xff000000, TJ_CENTRE, "Multiplier:  %.0f", multiplier_);
		font.RenderText(sprite_renderer, Vector3(800.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xff000000, TJ_CENTRE, "Height of Sundae:");
		font.RenderText(sprite_renderer, Vector3(760.0f, 250.0f, 0.0f), 1.5f, 0xff000000, TJ_CENTRE, "%.2f m", height_score_);
		font.RenderText(sprite_renderer, Vector3(700.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xff000000, TJ_CENTRE, "Final Score: ");
		font.RenderText(sprite_renderer, Vector3(650.0f, HALF_DISPLAY_HEIGHT, 0.0f), 1.0f, 0xff000000, TJ_CENTRE, "%.1f x %.0f", height_score_, multiplier_);
		font.RenderText(sprite_renderer, Vector3(620.0f, 250.0f, 0.0f), 1.5f, 0xff000000, TJ_CENTRE, "%.0f !!", score_);
		font.RenderText(sprite_renderer, Vector3(175.0f, 240.0f, -0.9f), 1.0f, 0xff000000, TJ_CENTRE, "YEY!");
	}

	if (chef_timer_ > 0)
	{
		if (chef_called_once_)
		{
			font.RenderText(sprite_renderer, Vector3(260.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "Chef:");
			font.RenderText(sprite_renderer, Vector3(200.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "STOP");
			font.RenderText(sprite_renderer, Vector3(180.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "DROPPING");
			font.RenderText(sprite_renderer, Vector3(160.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "THE");
			font.RenderText(sprite_renderer, Vector3(140.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "ICECREAM!!");
		}
		if (chef_called_twice_)
		{
			font.RenderText(sprite_renderer, Vector3(260.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "Chef:");
			font.RenderText(sprite_renderer, Vector3(210.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "IF YOU");
			font.RenderText(sprite_renderer, Vector3(190.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "DON'T");
			font.RenderText(sprite_renderer, Vector3(170.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "STOP");
			font.RenderText(sprite_renderer, Vector3(150.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "YOU'RE!!");
			font.RenderText(sprite_renderer, Vector3(130.0f, 265.0f, -0.9f), 0.9f, 0xff000000, TJ_CENTRE, "FIRED!!");
		}
	}
}



// ***************************************************
// On Exit function
// Called when the GameState is exited, lets sprite application know to change the state to the new state
// Acts as a clean up function and deletes the Box2d world and all objects
// ***************************************************

void GameState::OnExit(int new_state)
{	
	in_state_ = false;
	state_changed_ = true;
	new_state_ = new_state;

	// Clean up objects
	delete world_;
	world_ = NULL;
	delete scoop_manager_;
	delete spoon_manager_;
}



// ***************************************************
// Initilise Sprites Function
// Sets textures for all the objects in the game and sets up the sprites included in the Game State
// ***************************************************

void GameState::InitialiseSprites()
{
	// Set textures 
	if (hard_difficulty_on_)
	{
		CreatePins();
		for (int i = 0; i < AMOUNT_OF_PINS; i++)
		{
			pins_[i]->pin_sprite_.GiveTexture(spritesheet_);
			pins_[i]->SetTexture();
		}
	}

	for (int i = 0; i < MAX_SPOONS; i++)
	{
		spoon_manager_->spoons[i]->spoon_sprite_.GiveTexture(spritesheet_);
		spoon_manager_->spoons[i]->SetTexture();
	}

	for (int i = 0; i < scoop_manager_->current_scoop_amount_; i++)
	{
		if (scoop_manager_->scoop[i]->scoop_sprite_.type_ == GameObject::SCOOP)
		{
			scoop_manager_->scoop[i]->scoop_sprite_.GiveTexture(spritesheet_);
			scoop_manager_->scoop[i]->SetTexture();
		}
	}

	scoop_manager_->cherry_->scoop_sprite_.GiveTexture(spritesheet_);
	scoop_manager_->cherry_->SetTexture();

	// Background Sprite
	background_.set_width(DISPLAY_WIDTH);
	background_.set_height(DISPLAY_HEIGHT);
	background_.GiveTexture(spritesheet_);
	background_.TextureSettings(Vector2(0.5, 0.0), 0.25, 0.15);
	background_.set_position(Vector3(HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT, 0.0f));

	// Plate Sprite
	plate_sprite_.set_width(100.0f);
	plate_sprite_.set_height(350.0f);
	plate_sprite_.GiveTexture(spritesheet_);
	plate_sprite_.TextureSettings(Vector2(0.2, 0.65), 0.075, 0.2);

	// Ground Sprite
	ground_sprite_.set_width(20.0f);
	ground_sprite_.set_height(DISPLAY_WIDTH);

	// Ceiling Sprite
	ceiling_sprite_.set_width(20.0f);
	ceiling_sprite_.set_height(DISPLAY_WIDTH);

	// Stand Sprite
	stand_sprite_.set_width(180.0f);
	stand_sprite_.set_height(90.0f);
	stand_sprite_.GiveTexture(spritesheet_);
	stand_sprite_.TextureSettings(Vector2(0.3f, 0.75f), 0.1f, 0.05f);

	// Chef Sprite
	chef_sprite_.set_width(250.0f);
	chef_sprite_.set_height(350.0f);
	chef_sprite_.GiveTexture(spritesheet_);
	chef_sprite_.TextureSettings(Vector2(0.2f, 0.85f), 0.1f, 0.15f);
	chef_sprite_.set_position(Vector3(-300.0f, -300.0f, 0.0f));

	// Line sprite
	line_sprite_.set_height(DISPLAY_HEIGHT);
	line_sprite_.set_width(50.0f);
	line_sprite_.GiveTexture(spritesheet_);
	line_sprite_.TextureSettings(Vector2(0.3f, 0.4f), 0.02f, 0.25f);
	line_sprite_.set_position(Vector3(-10.0f, HALF_DISPLAY_HEIGHT, 0.0f));

	// Cherry Sprite & Finish Box Sprite
	finish_box_position_ = Vector3(550.0f, 500.0f, 0.0f);
	finish_box_.set_width(90.0f);
	finish_box_.set_height(90.0f);
	finish_box_.GiveTexture(spritesheet_);
	finish_box_.TextureSettings(Vector2(0.85, 0.25), 0.1, 0.1);
	finish_box_.set_position(finish_box_position_);

	// Menu Sprite
	menu_sprite_.set_width(70.0f);
	menu_sprite_.set_height(150.0f);
	menu_sprite_.GiveTexture(spritesheet_);
	menu_sprite_.TextureSettings(Vector2(0.9, 0.05), 0.05, 0.1);
	menu_sprite_.set_position(Vector3(900.0f, 90.0f, 0.0f));
}



// ***************************************************
// Create Pins function
// Called when hard difficulty is on, creates 5 pins passing in positions for the pins
// ***************************************************

void GameState::CreatePins()
{
	pins_[0] = new Pin(world_, 750.0f, 175.0f);
	pins_[1] = new Pin(world_, 750.0f, 355.0f);
	pins_[2] = new Pin(world_, 500.0f, HALF_DISPLAY_HEIGHT);
	pins_[3] = new Pin(world_, 320.0f, 125.0f);		
	pins_[4] = new Pin(world_, 320.0f, 415.0f);
}



// ***************************************************
// Handle Cherry function
// Checks the amount of scoops the player has used and determines when to allow the player to finish the game
// Moves Cherry onto screen and allows player to click the finish box
// Notifies game update that the cherry has been used and the game is over
// ***************************************************

void GameState::HandleCherry()
{
	// If the player has used a certain amount of scoops, let the player finish the game if they wish
	if (scoop_manager_->current_scoop_amount_ > 15)
	{
		can_call_cherry_ = true;
	}

	// Check if the player has selected the finish box
	if ((can_call_cherry_) && (!cherry_called_))
	{
		if ((OptionSelected(finish_box_, front_touch_)) && (front_touch_->type == TT_NEW))
		{
			cherry_called_ = true;
			call_cherry_ = true;
			can_call_cherry_ = false;
		}
	}

	// Set the cherry to active
	if (call_cherry_)
	{
		scoop_manager_->cherry_active_ = true;
		scoop_manager_->cherry_->scoop_->SetTransform(b2Vec2(GFX_BOX2D_POS_X(900.0f), GFX_BOX2D_POS_Y(HALF_DISPLAY_HEIGHT)), 0.0f);
		call_cherry_ = false;
	}
	if (scoop_manager_->cherry_active_)
	{
		MoveScoops(scoop_manager_->cherry_);
	}
}



// ***************************************************
// Handle Chef function
// Determines when to make the chef appear based on the amount of scoops dropped, and which stage of the chef should appear
// Notifies the Draw function on when to make the chef appear on screen
// ***************************************************

void GameState::HandleChef()
{
	// Timer for chef appearing on screen
	if (chef_timer_ >= 0)
	{
		chef_timer_ --;	
	}

	if ((scoop_manager_->scoops_fallen_ == 3) && (!chef_called_once_))
	{
		first_call_chef_ = true;
	}
	else if ((scoop_manager_->scoops_fallen_ >= 6) && (!chef_called_twice_))
	{
		second_call_chef_ = true;
	}
	else if ((scoop_manager_->scoops_fallen_ >= 9) && (game_playing_))
	{
		// Game Over
		OnExit(State::GAMEOVER_STATE);
	}

	if (chef_timer_ == 0)
	{
		chef_sprite_.set_position(Vector3(-200.0f, -200.0f, 0.0f));
	}

	if (first_call_chef_)
	{
		chef_sprite_.set_position(Vector3(170.0f, 170.0f, 0.0f));
		chef_timer_ = 250;
		chef_called_once_ = true;
		first_call_chef_ = false;
	}

	if (second_call_chef_)
	{
		// Change chef texture
		chef_called_once_ = false;
		chef_sprite_.set_uv_position(Vector2(0.3, 0.85f));
		chef_sprite_.set_position(Vector3(170.0f, 170.0f, 0.0f));
		chef_timer_ = 250;
		chef_called_twice_ = true;
		second_call_chef_ = false;
	}
}



// ***************************************************
// Set user data for all function
// Goes through each body involved in the world and sets it's user data to the GameObject associated with the body
// And also gives each body a type, which allows for collision detection later
// ***************************************************

void GameState::SetUserDataForAll()
{
	// Contacts // Need to set the user data for all the objects
	
	plate_body_->SetUserData(&plate_sprite_);
	plate_sprite_.SetObjectType(GameObject::PLATE);

	SetUpScoops();
	stand_body_->SetUserData(&stand_sprite_);
	stand_sprite_.SetObjectType(GameObject::STAND);
	ground_body_->SetUserData(&ground_sprite_);
	ground_sprite_.SetObjectType(GameObject::GROUND);
	ceiling_body_->SetUserData(&ceiling_sprite_);
	ceiling_sprite_.SetObjectType(GameObject::CEILING);
}


// ***************************************************
// Set Up Scoops function
// Sets the user data for the scoops if there is scoops created but not yet initialised
// This is called at the start of the GameState but also when new scoops are created so they all have user data associated with them
// ***************************************************

void GameState::SetUpScoops()
{
	for (int i = 0; i < scoop_manager_->current_scoop_amount_; i++)
	{
		if (!scoop_manager_->scoop[i]->initialised_)
		{
			scoop_manager_->scoop[i]->scoop_->SetUserData(&scoop_manager_->scoop[i]->scoop_sprite_);
			{
				scoop_manager_->scoop[i]->scoop_sprite_.GiveTexture(spritesheet_);
				scoop_manager_->scoop[i]->SetTexture();
			}
			scoop_manager_->scoop[i]->initialised_ = true;
		}
	}
}




// ***************************************************
// Create bodies function
// Creates the initial bodies for the game which don't have their own class
// ***************************************************

void GameState::CreateBodies()
{
	// Plate Body
	plate_initial_position_ = b2Vec2(GFX_BOX2D_POS_X(130.0f), GFX_BOX2D_POS_Y(HALF_DISPLAY_HEIGHT));
	plate_dimensions_ = b2Vec2(GFX_BOX2D_SIZE(60.0f), GFX_BOX2D_SIZE(350.0f)); 
	body_def_.type = b2_dynamicBody;
	body_def_.position = plate_initial_position_;
	plate_body_ = world_->CreateBody(&body_def_);
	dynamic_box_.SetAsBox(0.5*plate_dimensions_.x, 0.5*plate_dimensions_.y);
	fixture_def_.shape = &dynamic_box_;
	fixture_def_.density = 0.01f;
	fixture_def_.friction = 5.0f;
	fixture_def_.restitution = 0.0f; 
	plate_body_->CreateFixture(&fixture_def_);

	// Ground body
	ground_position_ = b2Vec2(GFX_BOX2D_POS_X(0.0f), GFX_BOX2D_POS_Y(HALF_DISPLAY_HEIGHT));
	ground_ceiling_body_dimensions_ = b2Vec2(GFX_BOX2D_SIZE(20.0f), GFX_BOX2D_SIZE(1500.0f));
	ground_ceiling_def_.position = ground_position_;
	ground_body_ = world_->CreateBody(&ground_ceiling_def_);
	ground_ceiling_shape_.SetAsBox(0.5*ground_ceiling_body_dimensions_.x, 0.5 * ground_ceiling_body_dimensions_.y);
	ground_body_->CreateFixture(&ground_ceiling_shape_, 0.0f);
	ground_sprite_.set_position(Vector3(BOX2D_GFX_POS_X(ground_position_.x), BOX2D_GFX_POS_Y(ground_position_.y), 0.0f));

	// Ceiling Body
	ceiling_position_ = b2Vec2(GFX_BOX2D_POS_X(DISPLAY_WIDTH), GFX_BOX2D_POS_Y(HALF_DISPLAY_HEIGHT));
	ground_ceiling_def_.position = ceiling_position_;
	ceiling_body_ = world_->CreateBody(&ground_ceiling_def_);
	ceiling_body_->CreateFixture(&ground_ceiling_shape_, 0.0f);
	ceiling_sprite_.set_position(Vector3(BOX2D_GFX_POS_X(ceiling_position_.x), BOX2D_GFX_POS_Y(ceiling_position_.y), 0.0f));

	// Stand Body
	stand_position_ = b2Vec2(GFX_BOX2D_POS_X(10.0f), GFX_BOX2D_POS_Y(HALF_DISPLAY_HEIGHT));
	stand_dimensions_ = b2Vec2(GFX_BOX2D_SIZE(50.0f), GFX_BOX2D_SIZE(60.0f));
	stand_def_.position = stand_position_;
	stand_body_ = world_->CreateBody(&stand_def_);
	stand_shape_.SetAsBox(0.5*stand_dimensions_.x, 0.5*stand_dimensions_.y);
	stand_body_->CreateFixture(&stand_shape_, 0.0f);
	stand_sprite_.set_position(Vector3(BOX2D_GFX_POS_X(stand_position_.x), BOX2D_GFX_POS_Y(stand_position_.y), 0.0f));

	// Set values for revolute joint 
	revolute_joint_.revolute_joint_struct_.lower_angle = -0.3f;
	revolute_joint_.revolute_joint_struct_.upper_angle = 0.3f;
	revolute_joint_.revolute_joint_struct_.limit_on = false;
	revolute_joint_.revolute_joint_struct_.max_torque = 1000000000.0f;
	revolute_joint_.revolute_joint_struct_.motor_speed = 0.0f;
	revolute_joint_.revolute_joint_struct_.motor_on = true;

	// Friction Revolute Joint between Plate (Seesaw) and Stand 
	// Giant Max Torque value so that the plate will resist rotation until the max torque is exceeded 
	// (body_1, body_2, lower_angle, upper_angle, limit_on, max_torque, motor_speed, motor_on);
	revolute_joint_.CreateRevoluteJoint(world_, stand_body_, plate_body_);
}




// ***************************************************
// Set Up Screen function
// Creates edges around the screen in the appropriate places so that bodys/objects can't move off the screen at those positions
// Creates edges within the screen so spoons and scoops are restrained at first
// ***************************************************

void GameState::SetUpScreen()
{
	// REMEMBER: SCREEN HAS ROTATED
	// Therefore left of screen = original top of screen
	
	b2Body* edge_body = world_->CreateBody(&edge_body_def_);

	// Left side
	edge_vertex_1_ = b2Vec2(GFX_BOX2D_POS_X(DISPLAY_WIDTH), GFX_BOX2D_POS_Y(0.0f));
	edge_vertex_2_ = b2Vec2(GFX_BOX2D_POS_X(SCOOP_X_LIMIT), GFX_BOX2D_POS_Y(0.0f));					// So spoons can move through
	edge.Set(edge_vertex_1_, edge_vertex_2_);
	edge_body->CreateFixture(&edge, 0.0f);

	// Right Side
	edge_vertex_1_ = b2Vec2(GFX_BOX2D_POS_X(DISPLAY_WIDTH), GFX_BOX2D_POS_Y(DISPLAY_HEIGHT));
	edge_vertex_2_ = b2Vec2(GFX_BOX2D_POS_X(SCOOP_X_LIMIT), GFX_BOX2D_POS_Y(DISPLAY_HEIGHT));		// So spoons can move through
	edge.Set(edge_vertex_1_, edge_vertex_2_);
	edge_body->CreateFixture(&edge, 0.0f);

	// Internal side on the left (to restrain spoon/scoop movement at these positions)
	edge_vertex_1_ = b2Vec2(GFX_BOX2D_POS_X(SCOOP_X_LIMIT), GFX_BOX2D_POS_Y(130.0f));
	edge_vertex_2_ = b2Vec2(GFX_BOX2D_POS_X(590.0f), GFX_BOX2D_POS_Y(130.0f));		
	edge.Set(edge_vertex_1_, edge_vertex_2_);
	edge_body->CreateFixture(&edge, 0.0f);

	// Internal side on the right (to restrain spoon/scoop movement at these positions)
	edge_vertex_1_ = b2Vec2(GFX_BOX2D_POS_X(SCOOP_X_LIMIT), GFX_BOX2D_POS_Y(410.0f));
	edge_vertex_2_ = b2Vec2(GFX_BOX2D_POS_X(590.0f), GFX_BOX2D_POS_Y(410.0f));		
	edge.Set(edge_vertex_1_, edge_vertex_2_);
	edge_body->CreateFixture(&edge, 0.0f);
}




// ***************************************************
// Collision Detection function
// Checks through the list of contacts to detect collisions between bodies
// Passes the type of the game object associated with the user data of the bodies in contact to carry out the collision response
// ***************************************************

void GameState::DetectCollision()
{
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();
	
	// Collision Detection
	for(int contact_num = 0; contact_num < contact_count; contact_num++)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			GameObject* game_objectA = (GameObject*)bodyA->GetUserData();
			GameObject* game_objectB = (GameObject*)bodyB->GetUserData();

			if ((game_objectA) && (game_objectB)) // If it isn't null 
			{
				CollisionResponse(game_objectA, game_objectB);
				CollisionResponse(game_objectB, game_objectA);
			}
		}

	   // Get next contact point
	   contact = contact->GetNext();
	}
}



// ***************************************************
// Move Scoop function
// Takes in a pointer to a Scoops object and allows the player to move it using the touch screen
// Creates a mouse joint that uses the touch screens position to move the body of the scoop
// ***************************************************

void GameState::MoveScoops(Scoops *scoop)
{
	int touch_screen_position_x = front_touch_->position.x;
	int touch_screen_position_y = front_touch_->position.y;
	
	// Set up values for the mouse joint
	mouse_joint_.mouse_joint_struct_.max_force_ = 300.0f;

	if (scoop->can_move_)
	{
		if (front_touch_->type == TT_NEW)
		{

			GameObject scoop_sprite = scoop->scoop_sprite_;
			Vector3 object_position = scoop_sprite.GetPosition();
			int half_height = (0.5 * scoop_sprite.height());
			int half_width = (0.5 * scoop_sprite.width());

			// Checks to see if the touch position is not outside the sprite
			if	(!(((touch_screen_position_x) < (object_position.x - half_width))
				||	((touch_screen_position_x) > (object_position.x + half_width))
				||	((touch_screen_position_y) < (object_position.y - half_height))
				||	((touch_screen_position_y) > (object_position.y + half_height))) )
			{
				// Sets cherry to active if this is the scoop passed in and it is touched
				if (scoop->scoop_sprite_.scoop_type_ == GameObject::CHERRY)
				{
					scoop_manager_->cherry_->cherry_active_ = true;
				}
				if (sound_FX_on_)
				{
					sound_index_ = audio_manager_->PlaySample(pickup_scoop_ID_, false);
				}
				// Gets the scoop ID & uses that so only the scoop being touched is used
				scoop_number_ = scoop->scoop_ID_;	
				mouse_joint_.CreateMouseJoint(world_, ground_body_, scoop->scoop_);
				SetScoopPosition();
				scoop->ResetState();
				scoop->grabbed_ = true;
			}
		}
		else if ((front_touch_->type == TT_ACTIVE) && (scoop->grabbed_))
		{
			//scoop->ResetState();
			scoop->grabbed_ = true;
			SetScoopPosition();
		}
		else if ((front_touch_->type == TT_RELEASED) && (scoop->grabbed_))
		{
			scoop->ResetState();
			scoop->falling_ = true;
			// Checks if it is null first
			if (mouse_joint_.mouse_joint_ != NULL)
			{
				world_->DestroyJoint(mouse_joint_.mouse_joint_);
				// Makes mouse joint equal null so it can be used again with another scoop
				mouse_joint_.mouse_joint_ = NULL;
			}
		}
	}
}



// ***************************************************
// Update Scoop Sounds function
// If the Sound FX are on, plays sound depending on the scoops state
// ***************************************************

void GameState::UpdateScoopSounds()
{
	for (int i = 0; i < scoop_manager_->current_scoop_amount_; i++)
	{
		if ((sound_FX_on_) && (scoop_manager_->scoop[i]->explosion_now_))
		{
			sound_index_ = audio_manager_->PlaySample(bomb_explosion_ID_, false);
		}
	}
}



// ***************************************************
// End Game Sequence
// Handles what happens when the game is completed
// ***************************************************

void GameState::EndGameSequence()
{
	game_playing_ = false;
	line_position_ = line_sprite_.position();

	if (cherry_timer_ > 0)
	{
		cherry_timer_--;
	}

	// Timer for cherry to allow for scoop movement to settle before cherry position is checked
	if (cherry_timer_ <= 0)
	{
		sundae_height = (scoop_manager_->cherry_->gfx_scoop_position_.x + (0.5 * scoop_manager_->cherry_->scoop_sprite_.height()));
		height_score_ = (sundae_height / 100);
		cout << "score " << height_score_ << endl;
		
		// Moves measuring line to the top of the cherry sprite
		if (line_position_.x < sundae_height)
		{
			line_movement_++;
			line_sprite_.set_position(Vector3(line_movement_, HALF_DISPLAY_HEIGHT, 0.0f)); 
		}

		// When line reaches height, display the score
		if (line_position_.x >= sundae_height)
		{
			chef_sprite_.set_uv_position(Vector2(0.4, 0.85));
			chef_sprite_.set_position(Vector3(170.0f, 170.0f, 0.0f));
			display_score_ = true;
		}
	}

	if (display_score_)
	{
		// Exits Game State if menu button is pressed
		if (OptionSelected(menu_sprite_, front_touch_))
		{
			OnExit(State::MENU_STATE);
		}
	}
}



// ***************************************************
// Set Scoop Position function
// Checks that the touch screen position is within the area that the scoop is allowed to move to limit the mouse joint target
// and sets the mouse joint position to this to move the body of the object
// ***************************************************

void GameState::SetScoopPosition()
{
	if (front_touch_->position.x < SCOOP_X_LIMIT)
	{
		front_touch_->position.x = SCOOP_X_LIMIT;
	}

	if (mouse_joint_.mouse_joint_ != NULL)
	{
		b2Vec2 current_mouse_position = b2Vec2(GFX_BOX2D_POS_X(front_touch_->position.x), GFX_BOX2D_POS_Y(front_touch_->position.y));
		mouse_joint_.mouse_joint_->SetTarget(current_mouse_position);
	}
}



// ***************************************************
// Collision Response Function
// Takes the user data (game objects) of the two bodies that collided within the DetectCollision function
// Carries out the appropriate collision response depending on the objects collided
// ***************************************************

void GameState::CollisionResponse(GameObject *collision_bodyA, GameObject *collision_bodyB)
{
	// Initialises variables for scoop collisions
	int scoop_number = 0;
	int bomb_number = 0;
	int normal_number = 0;
	int spoon_number;
	// Variables for two scoop collisions
	bool two_scoops_ = false;
	GameObject *scoop_body_;
	GameObject *second_scoop_body_;
	 
	if ((collision_bodyA->type_ == GameObject::SCOOP) && (collision_bodyB->type_ == GameObject::SCOOP))
	{
		two_scoops_ = true;
	}

	// Here set the scoop_body game object to the appropriate collision body
	if(!two_scoops_)
	{
		if (collision_bodyA->type_ == GameObject::SCOOP)
		{
			scoop_body_ = collision_bodyA;
		}
		if (collision_bodyB->type_ == GameObject::SCOOP)
		{
			scoop_body_ = collision_bodyB;
		}
	}

	// Here set the scoop_body_ and second_scoop_body_ to the appropriate collision bodies if two scoops collide
	// This is so you can differentiate between the two scoops
	if (two_scoops_)
	{
		scoop_body_ = collision_bodyA;
		second_scoop_body_ = collision_bodyB;
	}
	
	// Compare the ID stored within the GameObject information of the scoop to determine which specific scoop it is
	for (int i = 0; i < scoop_manager_->current_scoop_amount_; i++)
	{
		if (scoop_body_->ID_ == scoop_manager_->scoop[i]->scoop_sprite_.ID_)
		{
			scoop_number = scoop_body_->ID_;		
		}

		// If two scoops, and one is a bomb scoop, set the IDs to the appropriate numbers to identify which scoop is which
		if (two_scoops_)
		{
			if ((scoop_manager_->scoop[i]->bomb_scoop_) && (scoop_body_->ID_ == scoop_manager_->scoop[i]->scoop_sprite_.ID_))	
			{
				bomb_number = scoop_body_->ID_;		
			}
			if ((scoop_manager_->scoop[i]->normal_scoop_) && (second_scoop_body_->ID_ == scoop_manager_->scoop[i]->scoop_sprite_.ID_))
			{
				normal_number = second_scoop_body_->ID_;
			}
		}
	}


	// Collision response between Scoop and Plate

	if ((collision_bodyA->type_ == GameObject::SCOOP) && (collision_bodyB->type_ == GameObject::PLATE))
	{
		if (!scoop_manager_->scoop[scoop_number]->already_placed_)
		{
			if (sound_FX_on_)
			{
				sound_index_ = audio_manager_->PlaySample(landed_ID_, false);
			}
			scoop_score_++;
			spoon_manager_->scoops_placed_++;
			scoop_manager_->ScoopPlaced(scoop_number);
		}

		// For cherry
		if (collision_bodyA->scoop_type_ == GameObject::CHERRY)
		{
			if (!cherry_placed_)
			{
				cherry_timer_ = 300;
				cherry_placed_ = true;
			}
		}
	}


	// Collision Response between two scoops

	if ((collision_bodyA->type_ == GameObject::SCOOP) && (collision_bodyB->type_ == GameObject::SCOOP))
	{
		// Bomb collision
		if ((collision_bodyA->scoop_type_ == GameObject::BOMB_SCOOP) && (collision_bodyB->scoop_type_ == GameObject::NORMAL_SCOOP))
		{
			scoop_manager_->ScoopCollidesWithBomb(scoop_manager_->scoop[bomb_number], scoop_manager_->scoop[normal_number]);
		}

		// For cherry
		if (collision_bodyA->scoop_type_ == GameObject::CHERRY)
		{
			if (!cherry_placed_)
			{
				cherry_timer_ = 300;
				cherry_placed_ = true;
			}
		}

		// Two General Scoops
		if (!scoop_manager_->scoop[scoop_number]->already_placed_)
		{
			if(sound_FX_on_)
			{
				sound_index_ = audio_manager_->PlaySample(landed_ID_, false);
			}
			scoop_score_++;
			spoon_manager_->scoops_placed_++;
			scoop_manager_->ScoopPlaced(scoop_number);
		}
	}


	// Collision Response between Scoop and Ground

	if ((collision_bodyA->type_ == GameObject::SCOOP) && (collision_bodyB->type_ == GameObject::GROUND))
	{
		// Game over if cherry hits the floor
		if ((collision_bodyA->scoop_type_ == GameObject::CHERRY) && (!display_score_))
		{
			OnExit(State::GAMEOVER_STATE);
		}
		
		// If it hasn't already collided with the floor
		if (scoop_manager_->scoop[scoop_number]->can_collide_with_floor_)
		{
			if (!scoop_manager_->scoop[scoop_number]->already_placed_)
			{
				spoon_manager_->scoops_placed_++;
			}
			if (sound_FX_on_)
			{
				sound_index_ = audio_manager_->PlaySample(landed_ID_, false);
			}
			scoop_manager_->scoops_fallen_++;
			scoop_manager_->ScoopPlacedOnGround(scoop_number);
		}
	}


	// Collision response between Ceiling and Spoon (to detect when to set the spoons back to the beginning)

	if ((collision_bodyA->type_ == GameObject::CEILING) && (collision_bodyB->type_ == GameObject::SPOON))
		//|| ((collision_bodyA->type_ == GameObject::SPOON) && (collision_bodyB->type_ == GameObject::CEILING)))
	{
		// Detects which spoon has collided
		for (int i = 0; i < MAX_SPOONS; i++)
		{
			// Check which spoon it is
			// Set at - 50 because the ID's of the spoon are set to 50+
			if (collision_bodyA->ID_ == (spoon_manager_->spoons[i]->spoon_sprite_.ID_))
			{
				spoon_number = collision_bodyA->ID_ - 50;		
			}
			else if (collision_bodyB->ID_ == (spoon_manager_->spoons[i]->spoon_sprite_.ID_))
			{
				spoon_number = collision_bodyB->ID_ - 50;	
			}
		}

		if (spoon_manager_->spoons[spoon_number]->collide_with_ceiling_)
		{
			spoon_manager_->spoons_finished_++;
			spoon_manager_->spoons[spoon_number]->collide_with_ceiling_ = false;
		}
	}
}

