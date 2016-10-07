#include "spoon_manager.h"



// ***************************************************
// Spoon Manager Constructor
// Initialises everything for the spoon manager, and creates four spoons for the game
// ***************************************************

SpoonManager::SpoonManager(b2World* world_, Texture *spritesheet)
{	
	spritesheet_ = spritesheet;
	InitialiseConveyorBelts();
	scoops_placed_ = 0;
	spoons_finished_ = 0;
	spoons_moving_ = false;	

	// Static Reference Body initialisation
	reference_position_ = b2Vec2(GFX_BOX2D_POS_X(0.0f), GFX_BOX2D_POS_Y(HALF_DISPLAY_HEIGHT));
	reference_dimensions_ = b2Vec2(GFX_BOX2D_SIZE(10), GFX_BOX2D_SIZE(10.0f));
	reference_def_.position = reference_position_;
	reference_body_ = world_->CreateBody(&reference_def_);
	reference_shape_.SetAsBox(0.5*reference_dimensions_.x, 0.5 * reference_dimensions_.y);
	reference_body_->CreateFixture(&reference_shape_, 0.0f);

	// Creates the four spoons needed passing in the world object so the spoon class can create a body
	// Passes in the appropriate starting positions needed for the spoons and bools to say if they are a left or right spoon
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			spoons[i] = new Spoon(world_, true, false, 700.0f, 63.0f);	
			spoons[i]->move_to_second_ = true;	
		}
		if (i == 1)
		{
			spoons[i] = new Spoon(world_, true, false, 650.0f, -75.0f);
			spoons[i]->move_to_first_ = true;
		}
		if (i == 2)
		{
			spoons[i] = new Spoon(world_, false, true, 700.0f, 477.0f);
			spoons[i]->move_to_second_ = true;
		}
		if (i == 3)
		{
			spoons[i] = new Spoon(world_, false, true, 650.0f, 605.0f);
			spoons[i]->move_to_first_ = true;
		}

		spoons[i]->spoon_sprite_.SetObjectType(GameObject::SPOON);
		// Gives it an ID above 50 so it doesn't overlap with the scoop game object IDs
		spoons[i]->spoon_sprite_.GiveID(50 + i);
		spoons[i]->SetUpMouseJoint(reference_body_, world_);
	}
}



// ***************************************************
// Initialise Conveyor Belts Function
// Initialises these game objects and gives them information for animation
// ***************************************************

void SpoonManager::InitialiseConveyorBelts()
{
	// Conveyor Belts
	conveyor_belt_1_.set_width(400.0f);
	conveyor_belt_1_.set_height(100.0f);
	conveyor_belt_1_.GiveTexture(spritesheet_);
	conveyor_belt_1_.TextureSettings(Vector2(0.35, 0.4), 0.1, 0.05);
	conveyor_belt_1_.set_position(Vector3(750.0f, 50.0f, 0.0f));
	conveyor_belt_1_.game_frame_count_ = 10;
	conveyor_belt_1_.num_frames_ = 2;

	conveyor_belt_2_.set_width(400.0f);
	conveyor_belt_2_.set_height(100.0f);
	conveyor_belt_2_.GiveTexture(spritesheet_);
	conveyor_belt_2_.TextureSettings(Vector2(0.35, 0.4), 0.1, 0.05);
	conveyor_belt_2_.set_position(Vector3(750.0f, 580.0f, 0.0f));
	conveyor_belt_2_.game_frame_count_ = 10;
	conveyor_belt_2_.num_frames_ = 2;
}



// ***************************************************
// Update Function
// Detects where spoons are at, and uses information from GameState to determine when spoons are moving and when new scoops need to be created
// ***************************************************

void SpoonManager::Update(ScoopManager *scoop_manager)
{
	for (int i = 0; i < MAX_SPOONS; i++)
	{
		// Cycles through spoons and moves them to the next position when the two current scoops are placed
		if (scoops_placed_ == 2)
		{
			if (spoons[i]->at_second_)
			{
				spoons[i]->move_to_top_ = true;
			}
			if (spoons[i]->at_first_)
			{
				spoons[i]->move_to_second_ = true;
			}
			spoons_moving_ = true;
		}

		// When two spoons reach the ceiling they are then reset and new scoops are called 
		if (spoons_finished_ == 2)
		{
			if (spoons[i]->move_to_top_)
			{
				spoons[i]->move_delay_ = 20;
				spoons[i]->move_to_start_ = true;
			}
			// Passes information to the scoop manager to call two new scoops
			scoop_manager->call_new_scoops_ = true;
			spoons_moving_ = false;
			scoops_placed_ = 0;
		}
	}

	if (spoons_moving_)
	{
		conveyor_belt_1_.Animate();
		conveyor_belt_2_.Animate();
	}

	if (scoops_placed_ == 0)
	{
		spoons_finished_ = 0; 
	}

	for (int i = 0; i < MAX_SPOONS; i++)
	{
		spoons[i]->Update();
	}
}