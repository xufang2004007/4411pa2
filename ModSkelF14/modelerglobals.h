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
#define COLOR_BOY_BODY  0.5f, 0.6f, 1.0f
#define COLOR_GIRL_BODY  1.0f, 0.6f, 0.5f

// Common
	// Head
	#define HEAD_LENGTH	1.25
	#define HEAD_RADIUS	0.625
	#define EYES_COORD 0.25, 0.5, 0.125
	#define EYES_RADIUS 0.1

// Boy
	// Body
	#define BOY_BODY_X	2
	#define BOY_BODY_Y	0.75
	#define BOY_BODY_Z	2.25
	// Arm
	#define BOY_UPPER_ARM_LENGTH	1.375
	#define BOY_UPPER_ARM_RADIUS	2 * 0.2
	#define BOY_ELBOW_ANGLE	30 // to be variable
	#define BOY_LOWER_ARM_LENGTH	1.25
	#define BOY_LOWER_ARM_RADIUS	2 * 0.15
	#define BOY_SHOULDER_ANGLE -135 // to be variable
	// Head
	#define BOY_HEAD_X	1.125
	#define BOY_HEAD_Y	0.875
	#define BOY_HEAD_Z	1.25
	#define BOY_HEAD_PITCH_ANGLE	15 // to be variable
	#define BOY_HEAD_TILT_ANGLE	10 // to be variable
// Girl
	// Body
	#define GIRL_BODY_LENGTH	2
	#define GIRL_WAIST_RADIUS 0.75
	#define GIRL_SHOULDER_LENGTH 1 // actually half
	// Arm
	#define GIRL_UPPER_ARM_LENGTH	1.35
	#define GIRL_ELBOW_ANGLE	30 // to be variable
	#define GIRL_LOWER_ARM_LENGTH	1.225
	#define GIRL_SHOULDER_ANGLE -135 // to be variable
	#define GIRL_UPPER_ARM_RADIUS 0.2
	#define GIRL_LOWER_ARM_RADIUS 0.15
	// Head
	#define GIRL_HEAD_PITCH_ANGLE	15 // to be variable
	#define GIRL_HEAD_TILT_ANGLE	10 // to be variable
	// Leg
	#define GIRL_LEG_RADIUS 0.25
	#define GIRL_LEG_LENGTH 1.25

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif