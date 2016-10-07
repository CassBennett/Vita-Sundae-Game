#ifndef JOINTS_H
#define JOINTS_H

#include "Box2D\Box2D.h"

class Joints
{
public:
	// Revolute Joint Struct
	struct RevoluteJointStruct
	{
		float lower_angle;
		float upper_angle;
		bool limit_on;
		float max_torque;
		float motor_speed;
		bool motor_on;
	};

	struct MouseJointStruct
	{
		float max_force_;
	};

	// Joint Creation Functions
	void CreateRevoluteJoint(b2World* world, b2Body *body_1, b2Body *body_2);
	void CreateMouseJoint(b2World* world, b2Body *body_1, b2Body *body_2);

	// Joint Definitions/Variables
	b2RevoluteJointDef revolutionJointDef;
	b2MouseJointDef mouse_joint_def_;
	b2MouseJoint* mouse_joint_;
	bool mouse_joint_on_;
	MouseJointStruct mouse_joint_struct_;
	RevoluteJointStruct revolute_joint_struct_;
private:

};

#endif