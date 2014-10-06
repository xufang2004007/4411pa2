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
#define COLOR_BODY  0.7f, 0.6f, 0.5f

// Some constants
#define BODY_LENGTH	2
#define HEAD_LENGTH	1.25
#define HEAD_RADIUS	0.625
#define EYES_COORD 0.5, 1, 1
#define EYES_RADIUS 0.1
#define UPPER_ARM_LENGTH	1.25
#define LOWER_ARM_LENGTH	1
#define HEAD_PITCH_ANGLE	15 // to be variable
#define HEAD_TILT_ANGLE	10 // to be variable
#define ELBOW_ANGLE	-30

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif