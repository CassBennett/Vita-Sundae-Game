#include "joints.h"



// ***************************************************
// Create mouse joint function
// Mouse joint moves a body to a given position on the screen
// Takes in two objects. Body 1 is the reference body (it is the body that body 2 uses as reference to where it has moved) 
// which will usually be a static object. Body 2 is the body that will move. Also takes in the max force behind the movement
// ***************************************************

void Joints::CreateMouseJoint(b2World* world, b2Body *body_1, b2Body *body_2)
{
	mouse_joint_ = NULL;
	mouse_joint_def_.bodyA = body_1;					// Reference body
	mouse_joint_def_.bodyB = body_2;
	b2Vec2 bodypos = body_2->GetWorldCenter();
	body_2->SetAwake(true);
	mouse_joint_def_.target.Set(bodypos.x, bodypos.y);
	mouse_joint_def_.collideConnected = true;
	mouse_joint_def_.maxForce = mouse_joint_struct_.max_force_ * body_2->GetMass();
	mouse_joint_ = (b2MouseJoint*)world->CreateJoint(&mouse_joint_def_);
	mouse_joint_on_ = true;
}



// ***************************************************
// Create Revolute Joint function
// Revolute position rotates a body around another
// Takes in the two bodies involved in the revolute joint. Body 1 is the body that Body 2 rotates around
// Also takes in the lower & upper angle limit, flags to turn the limit and motor on, and values for max torque and motor speed
// ***************************************************

void Joints::CreateRevoluteJoint(b2World* world, b2Body *body_1, b2Body *body_2)
{
	revolutionJointDef.Initialize(body_1, body_2, body_1->GetPosition());
	revolutionJointDef.lowerAngle = revolute_joint_struct_.lower_angle * b2_pi;	
	revolutionJointDef.upperAngle = revolute_joint_struct_.upper_angle * b2_pi;	
	revolutionJointDef.enableLimit = revolute_joint_struct_.limit_on;
	revolutionJointDef.maxMotorTorque = revolute_joint_struct_.max_torque;			// Max force that can make the object rotate
	revolutionJointDef.motorSpeed = revolute_joint_struct_.motor_speed;			// The speed of the motor
	revolutionJointDef.enableMotor = revolute_joint_struct_.motor_on;	
	b2RevoluteJoint* revolutionJoint = (b2RevoluteJoint*)world->CreateJoint(&revolutionJointDef);
}