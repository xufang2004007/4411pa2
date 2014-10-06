#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	XPOS, YPOS, ZPOS,
	BOY_GIRL_BIND, BOY_GIRL_SIDE, BOY_GIRL_ANGLE,
	BOY_HEAD_PITCH, BOY_HEAD_TILT, BOY_SHOULDER_ANGLE, BOY_ELBOW_ANGLE,
	NUMCONTROLS
};

// Colors
#define COLOR_EYES	0.0f, 0.0f, 0.0f
#define COLOR_BOY_BODY  0.5f, 0.6f, 1.0f
#define COLOR_GIRL_BODY  1.0f, 0.6f, 0.5f

// Common
	// Misc.
	// BOY_GIRL_ANGLE						variable

// Boy
	// Body
	#define BOY_BODY_X							2
	#define BOY_BODY_Y							0.75
	#define BOY_BODY_Z							2.25
	// Arm
	#define BOY_UPPER_ARM_LENGTH				1.375
	#define BOY_UPPER_ARM_RADIUS				2 * 0.2
		 // BOY_ELBOW_ANGLE						variable
	#define BOY_LOWER_ARM_LENGTH				1.25
	#define BOY_LOWER_ARM_RADIUS				2 * 0.15
		 // BOY_SHOULDER_ANGLE					variable
	// Head
	#define BOY_HEAD_X							1.125
	#define BOY_HEAD_Y							0.875
	#define BOY_HEAD_Z							1.25
		// BOY_HEAD_PITCH						variable
		// BOY_HEAD_TILT						variable
	// Leg
	#define BOY_LEG_LENGTH						1.25
	#define BOY_LEFT_UPPER_LEG_PITCH_ANGLE		45
	#define BOY_LEFT_UPPER_LEG_RAW_ANGLE		0
	#define BOY_LEFT_UPPER_LEG_YAW_ANGLE		0
	#define BOY_RIGHT_UPPER_LEG_PITCH_ANGLE		-15
	#define BOY_RIGHT_UPPER_LEG_RAW_ANGLE		0
	#define BOY_RIGHT_UPPER_LEG_YAW_ANGLE		0
	#define BOY_LEFT_LOWER_LEG_PITCH_ANGLE		-45
	#define BOY_RIGHT_LOWER_LEG_PITCH_ANGLE		-45
	// Foot
	#define BOY_LEFT_FOOT_PITCH_ANGLE			-0
	#define BOY_RIGHT_FOOT_PITCH_ANGLE			-30

// Girl
	// Body
	#define GIRL_BODY_LENGTH					2
	#define GIRL_WAIST_RADIUS					0.75
	#define GIRL_SHOULDER_LENGTH				1 // actually half
	#define GIRL_BREAST_RADIUS					0.5
	// Arm
	#define GIRL_UPPER_ARM_LENGTH				1.35
	#define GIRL_ELBOW_LEFT_ANGLE				30 // to be variable
	#define GIRL_ELBOW_RIGHT_ANGLE				30 // to be variable
	#define GIRL_LOWER_ARM_LENGTH				1.225
	#define GIRL_SHOULDER_LEFT_ANGLE_FLAP		15 // to be variable
	#define GIRL_SHOULDER_RIGHT_ANGLE_FLAP		15 // to be variable
	#define GIRL_SHOULDER_LEFT_ANGLE_RAISE		15 // to be variable
	#define GIRL_SHOULDER_RIGHT_ANGLE_RAISE		15 // to be variable
	#define GIRL_SHOULDER_LEFT_ANGLE_AXIAL		15 // to be variable
	#define GIRL_SHOULDER_RIGHT_ANGLE_AXIAL		15 // to be variable
	#define GIRL_UPPER_ARM_RADIUS				0.2
	#define GIRL_LOWER_ARM_RADIUS				0.15
	// Head
	#define GIRL_HEAD_PITCH_ANGLE				15 // to be variable
	#define GIRL_HEAD_TILT_ANGLE				10 // to be variable
	#define GIRL_HEAD_LENGTH					1.25
	#define GIRL_HEAD_RADIUS					0.625
	#define GIRL_EYES_COORD						0.25, 0.5, 0.125
	#define GIRL_EYES_RADIUS					0.1
	// Leg
	#define GIRL_LEG_RADIUS						0.2
	#define GIRL_LEG_LENGTH						1.25
	#define GIRL_LEFT_UPPER_LEG_PITCH_ANGLE		45
	#define GIRL_LEFT_UPPER_LEG_RAW_ANGLE		0
	#define GIRL_LEFT_UPPER_LEG_YAW_ANGLE		0
	#define GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE	-15
	#define GIRL_RIGHT_UPPER_LEG_RAW_ANGLE		0
	#define GIRL_RIGHT_UPPER_LEG_YAW_ANGLE		0
	#define GIRL_LEFT_LOWER_LEG_PITCH_ANGLE		-45
	#define GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE	-45
	// Foot
	#define GIRL_LEFT_FOOT_PITCH_ANGLE			-0
	#define GIRL_RIGHT_FOOT_PITCH_ANGLE			-30

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

// do NOT use glPushMatrix() directly, as you might forget to pop them. Also, it will cause the indentation to corrupt with some IDEs.
// use this instead
#define GLMATRIX(stmt) { glPushMatrix(); stmt; glPopMatrix(); }

#endif