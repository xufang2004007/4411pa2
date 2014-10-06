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
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f // @deprecated
#define COLOR_GREEN		0.0f, 1.0f, 0.0f // @deprecated
#define COLOR_BLUE		0.0f, 0.0f, 1.0f // @deprecated
#define COLOR_EYES	0.0f, 0.0f, 0.0f
#define COLOR_GIRL_BODY  1.0f, 0.6f, 0.4f

// Common
	// Head
	#define HEAD_LENGTH	1.25
	#define HEAD_RADIUS	0.625
	#define EYES_COORD 0.25, 0.5, 0.125
	#define EYES_RADIUS 0.1

// Girl
	// Body
	#define GIRL_BODY_LENGTH	2
	#define GIRL_WAIST_RADIUS 0.75
	#define GIRL_SHOULDER_LENGTH 1 // actually half
	#define GIRL_BREAST_RADIUS 0.5
	// Arm
	#define GIRL_UPPER_ARM_LENGTH	1.375
	#define GIRL_ELBOW_ANGLE	30 // to be variable
	#define GIRL_LOWER_ARM_LENGTH	1.25
	#define GIRL_SHOULDER_ANGLE -135
	#define GIRL_UPPER_ARM_RADIUS 0.225
	#define GIRL_LOWER_ARM_RADIUS 0.175
	// Head
	#define GIRL_HEAD_PITCH_ANGLE	15 // to be variable
	#define GIRL_HEAD_TILT_ANGLE	10 // to be variable
	// Leg
	#define GIRL_LEG_RADIUS 0.25
	#define GIRL_LEG_LENGTH 1.25
	#define GIRL_LEFT_UPPER_LEG_PITCH_ANGLE 45
	#define GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE -15
	#define GIRL_LEFT_LOWER_LEG_PITCH_ANGLE -45
	#define GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE -45
	//Foot
	#define GIRL_LEFT_FOOT_PITCH_ANGLE -0
	#define GIRL_RIGHT_FOOT_PITCH_ANGLE -30


// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif