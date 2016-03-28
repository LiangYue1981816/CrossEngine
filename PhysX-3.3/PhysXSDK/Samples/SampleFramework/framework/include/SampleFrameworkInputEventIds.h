/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
#ifndef _SAMPLE_FRAMEWORK_INPUT_EVENT_IDS_H
#define _SAMPLE_FRAMEWORK_INPUT_EVENT_IDS_H

static const float GAMEPAD_ROTATE_SENSITIVITY = 0.7f;
static const float GAMEPAD_DEFAULT_SENSITIVITY = 1.0f;

// InputEvents used by SampleApplication
enum SampleFrameworkInputEventIds
{
	CAMERA_SHIFT_SPEED = 0,
	CAMERA_MOVE_LEFT,
	CAMERA_MOVE_RIGHT,
	CAMERA_MOVE_UP,
	CAMERA_MOVE_DOWN,
	CAMERA_MOVE_FORWARD,
	CAMERA_MOVE_BACKWARD,
	CAMERA_SPEED_INCREASE,
	CAMERA_SPEED_DECREASE,

	CAMERA_MOUSE_LOOK,
	CAMERA_MOVE_BUTTON,

	CAMERA_GAMEPAD_ROTATE_LEFT_RIGHT,
	CAMERA_GAMEPAD_ROTATE_UP_DOWN,
	CAMERA_GAMEPAD_MOVE_LEFT_RIGHT,
	CAMERA_GAMEPAD_MOVE_FORWARD_BACK,

	CAMERA_JUMP,
	CAMERA_CROUCH,
	CAMERA_CONTROLLER_INCREASE,
	CAMERA_CONTROLLER_DECREASE,

	NUM_SAMPLE_FRAMEWORK_INPUT_EVENT_IDS, 
};

// InputEvent descriptions used by SampleApplication
const char* const SampleFrameworkInputEventDescriptions[] =
{
	"change the camera speed",
	"move the camera left", 
	"move the camera right",
	"move the camera up",
	"move the camera down",
	"move the camera forward",
	"move the camera backward",
	"increase the camera move speed",
	"decrease the camera move speed",

	"look around with the camera",
	"enable looking around with the camera",

	"look left and right with the camera",
	"look up and down look with the camera",
	"move the camera left and right",
	"move the camera forward and backward",

	"jump",
	"crouch",
	"next controller",
	"previous controller",
};

PX_COMPILE_TIME_ASSERT(PX_ARRAY_SIZE(SampleFrameworkInputEventDescriptions) == NUM_SAMPLE_FRAMEWORK_INPUT_EVENT_IDS);

#endif

