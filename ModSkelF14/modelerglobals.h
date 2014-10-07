#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// Controls

enum SampleModelControls { 
	XPOS, YPOS, ZPOS,
	BOY_GIRL_SIDE, BOY_GIRL_ANGLE,
	BOY_HEAD_PITCH, BOY_HEAD_TILT, BOY_SHOULDER_ANGLE, BOY_ELBOW_ANGLE,
	BOY_LEFT_UPPER_LEG_PITCH_ANGLE, BOY_RIGHT_UPPER_LEG_PITCH_ANGLE, BOY_LEFT_UPPER_LEG_ROLL_ANGLE, BOY_RIGHT_UPPER_LEG_ROLL_ANGLE, BOY_LEFT_UPPER_LEG_YAW_ANGLE, BOY_RIGHT_UPPER_LEG_YAW_ANGLE,
	BOY_LEFT_LOWER_LEG_PITCH_ANGLE, BOY_RIGHT_LOWER_LEG_PITCH_ANGLE, BOY_LEFT_FOOT_PITCH_ANGLE, BOY_RIGHT_FOOT_PITCH_ANGLE, 
	GIRL_HEAD_PITCH, GIRL_HEAD_TILT,
	GIRL_SHOULDER_LEFT_ANGLE_FLAP, GIRL_SHOULDER_RIGHT_ANGLE_FLAP, GIRL_SHOULDER_LEFT_ANGLE_RAISE, GIRL_SHOULDER_RIGHT_ANGLE_RAISE, GIRL_SHOULDER_LEFT_ANGLE_AXIAL, GIRL_SHOULDER_RIGHT_ANGLE_AXIAL,
	GIRL_ELBOW_LEFT_ANGLE, GIRL_ELBOW_RIGHT_ANGLE,
	NUMCONTROLS
};

// Colors

#define COLOR_EYES	0.0f, 0.0f, 0.0f
#define COLOR_BOY_BODY  0.5f, 0.6f, 1.0f
#define COLOR_GIRL_BODY  1.0f, 0.6f, 0.5f

// Dimensions (please, trail each integer with `.0` to make sure it is treated as a double)
	// Common
		// BOY_GIRL_ANGLE							variable
	// Boy
		// Body
		#define BOY_BODY_X							2.0
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
		#define BOY_UPPER_LEG_LENGTH				1.25
		#define BOY_UPPER_LEG_X						0.625
		#define BOY_UPPER_LEG_Y						0.5
		#define BOY_LOWER_LEG_LENGTH				1.25
		#define BOY_LOWER_LEG_RADIUS				0.5
			 // BOY_LEFT_UPPER_LEG_PITCH_ANGLE		variable
			 // BOY_RIGHT_UPPER_LEG_PITCH_ANGLE		variable
			 // BOY_LEFT_UPPER_LEG_ROLL_ANGLE		variable
			 // BOY_RIGHT_UPPER_LEG_ROLL_ANGLE		variable
			 // BOY_LEFT_UPPER_LEG_YAW_ANGLE		variable
			 // BOY_RIGHT_UPPER_LEG_YAW_ANGLE		variable
			 // BOY_LEFT_LOWER_LEG_PITCH_ANGLE		variable		 
			 // BOY_RIGHT_LOWER_LEG_PITCH_ANGLE		variable
		// Foot
		#define BOY_FOOT_LENGTH						0.875
		#define BOY_FOOT_WIDTH						0.5
		#define BOY_FOOT_THICKNESS					0.175 
			 // BOY_LEFT_FOOT_PITCH_ANGLE			variable
			 // BOY_RIGHT_FOOT_PITCH_ANGLE			variable
	// Girl
		// Body
		#define GIRL_BODY_LENGTH					2.0
		#define GIRL_WAIST_RADIUS					0.75
		#define GIRL_SHOULDER_LENGTH				1.0
		#define GIRL_BREAST_RADIUS					0.5
		// Arm
		#define GIRL_UPPER_ARM_LENGTH				1.35
			 // GIRL_ELBOW_LEFT_ANGLE				variable
			 // GIRL_ELBOW_RIGHT_ANGLE				variable
		#define GIRL_LOWER_ARM_LENGTH				1.225
			 // GIRL_SHOULDER_LEFT_ANGLE_FLAP		variable
			 // GIRL_SHOULDER_RIGHT_ANGLE_FLAP		variable
			 // GIRL_SHOULDER_LEFT_ANGLE_RAISE		variable
			 // GIRL_SHOULDER_RIGHT_ANGLE_RAISE		variable
			 // GIRL_SHOULDER_LEFT_ANGLE_AXIAL		variable
			 // GIRL_SHOULDER_RIGHT_ANGLE_AXIAL		variable
		#define GIRL_UPPER_ARM_RADIUS				0.2
		#define GIRL_LOWER_ARM_RADIUS				0.15
		// Head
			 // GIRL_HEAD_PITCH						variable
			 // GIRL_HEAD_TILT						variable
		#define GIRL_HEAD_LENGTH					1.25
		#define GIRL_HEAD_RADIUS					0.625
		#define GIRL_EYES_COORD						0.25, 0.5, 0.125
		#define GIRL_EYES_RADIUS					0.1
		// Leg
		#define GIRL_LEG_RADIUS						0.2
		#define GIRL_LEG_LENGTH						1.25
		#define GIRL_LEFT_UPPER_LEG_PITCH_ANGLE		45.0
		#define GIRL_LEFT_UPPER_LEG_ROLL_ANGLE		0.0
		#define GIRL_LEFT_UPPER_LEG_YAW_ANGLE		0.0
		#define GIRL_RIGHT_UPPER_LEG_PITCH_ANGLE	-15.0
		#define GIRL_RIGHT_UPPER_LEG_ROLL_ANGLE		0.0
		#define GIRL_RIGHT_UPPER_LEG_YAW_ANGLE		0.0
		#define GIRL_LEFT_LOWER_LEG_PITCH_ANGLE		-45
		#define GIRL_RIGHT_LOWER_LEG_PITCH_ANGLE	-45
		// Foot
		#define GIRL_LEFT_FOOT_PITCH_ANGLE			-0.0
		#define GIRL_RIGHT_FOOT_PITCH_ANGLE			-30.0

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif