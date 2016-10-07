#include "scoop_manager.h"



// ***************************************************
// Scoop Manager Constructor
// Initialises everything for scoops. Creates the initial 4 scoops and the final cherry scoop and passes in initital positions for each
// Takes in a pointer to a world object to pass to scoops so they can create bodies
// ***************************************************

ScoopManager::ScoopManager(b2World* world)
{
	// Make all the potential scoops here
	srand (time(NULL));
	spoon_timer_ = 0;
	call_new_scoops_ = false;
	scoops_fallen_ = 0;
	current_scoop_amount_ = 0;

	cherry_ = new CherryScoop(world);
	cherry_->scoop_->SetUserData(&cherry_->scoop_sprite_);
	cherry_->scoop_sprite_.SetScoopType(GameObject::CHERRY);
	cherry_->scoop_sprite_.set_width(90.0f);
	cherry_active_ = false;

	for (int i = 0; i < INITIAL_SCOOPS; i++)
	{
		if (i == 0)
		{
			scoop[i] = new BombScoop(world, 730.0f, 70.0f, i);
		}
		if (i == 1)
		{
			scoop[i] = new NormalScoop(world, 730.0f, 460.0f, i);
		}
		if (i == 2)
		{
			scoop[i] = new NormalScoop(world, 675.0f, -25.0f, i);
		}
		if (i == 3)
		{
			scoop[i] = new NormalScoop(world, 675.0f, 590.0f, i);
		}
		scoop[i]->scoop_sprite_.GiveID(i);
		current_scoop_amount_++;
	}
}



// ***************************************************
// Update function
// Updates all of the scoops individually and detects when new scoops need to be created, and calls the New Scoops function
// ***************************************************

void ScoopManager::Update(b2World* world)
{
	for (int i = 0; i < current_scoop_amount_; i++)
	{
		scoop[i]->UpdateAllScoops();
		scoop[i]->Update();
	}
	cherry_->UpdateAllScoops();
	cherry_->Update();
	
	// Spoon Manager sets call_new_scoops to true when two spoons have reached the ceiling 
	if (call_new_scoops_)
	{
		if ((current_scoop_amount_ + SCOOPS_ALLOWED) != NUM_OF_SCOOPS)
		{
			NewScoops(world);
		}
	}
}



// ***************************************************
// Scoop Placed function
// Carries out the collision response for a scoop being placed on the plate or on another scoop
// Takes in the ID of the scoop involved in the collision from the Collision Response function within GameState
// ***************************************************

void ScoopManager::ScoopPlaced(int scoop_ID)
{
	for (int i = 0; i < current_scoop_amount_; i++)
	{
		if (scoop[i]->scoop_ID_ == scoop_ID)
		{
			if (!scoop[scoop_ID]->scared_)
			{
				scoop[scoop_ID]->ResetState();
				scoop[scoop_ID]->sitting_ = true;
			}
			scoop[scoop_ID]->can_collide_with_plate_ = false;
			scoop[scoop_ID]->already_placed_ = true;
		}
	} 
}



// ***************************************************
// Scoop Placed On Ground
// Carries out the collision response for a scoop being placed on the ground
// Takes in the ID of the scoop involved in the collision from the Collision Response function within GameState
// ***************************************************

void ScoopManager::ScoopPlacedOnGround(int scoop_ID)
{
	for (int i = 0; i < current_scoop_amount_; i++)
	{
		if (scoop[i]->scoop_ID_ == scoop_ID)
		{
			scoop[scoop_ID]->ResetState();
			scoop[scoop_ID]->sad_ = true;
			scoop[scoop_ID]->already_placed_ = true;
			// Sets the scoop type to ground so that if other scoops touch it they are spoiled and take away points too
			// Also so player can't stack scoops onto scoops already on the ground and get points
			scoop[scoop_ID]->scoop_sprite_.type_ = GameObject::GROUND;
			scoop[scoop_ID]->can_collide_with_floor_ = false;
		}
	}
}



// ***************************************************
// New Scoops function
// Every time 2 new scoops need to be created this function is called. It creates two more and increases the current scoop amount
// This is so the current scoop amount can be used in for loops within gamestate without accessing scoops that don't exist yet
// Takes in a pointer to a Box2d World object to pass into the scoop constructors
// ***************************************************

void ScoopManager::NewScoops(b2World* world)
{
	for (int i = current_scoop_amount_; i < (current_scoop_amount_ + SCOOPS_ALLOWED); i++)
	{
		if (i == current_scoop_amount_)
		{
			// Therefore every 8th scoop is a bomb
			if (i % 8 == 0)
			{
				scoop[i] = new BombScoop(world, 675.0f, -25.0f, i);
			}
			else
			{
				scoop[i] = new NormalScoop(world, 675.0f, -25.0f, i);
			}
		}

		if (i == current_scoop_amount_ + 1)
		{
			scoop[i] = new NormalScoop(world, 675.0f, 590.0f, i);
		}
		scoop[i]->scoop_sprite_.GiveID(i);
	}

	// Updates current scoop amount
	current_scoop_amount_ += SCOOPS_ALLOWED; 
	call_new_scoops_ = false;
}



// ***************************************************
// Scoop Collides With Bomb function
// Takes in the bomb scoop and the normal scoop involved in the collision
// Carries out the collision response between these two scoops if an explosion happens
// ***************************************************

void ScoopManager::ScoopCollidesWithBomb(Scoops *bomb_scoop, Scoops *normal_scoop)
{
	if (bomb_scoop->exploding_) 
	{
		normal_scoop->ResetState();
		normal_scoop->scared_ = true;
	}

	if (bomb_scoop->explosion_now_)
	{
		normal_scoop->AffectedByBomb(bomb_scoop->gfx_scoop_position_);
	}
}